#include "MemoryObject.h"

void DMAMem::MemoryObject::registerOffset(int offset, void* destination, int typeSize)
{
	OffsetEntry oe = OffsetEntry();
	oe.offset = offset;
	oe.destination = destination;
	oe.typeSize = typeSize;
	offsetVector.push_back(oe);
}

void DMAMem::MemoryObject::registerPointer(int offset, MemoryObject* destination)
{
	std::shared_ptr<OffsetPointer> op(new OffsetPointer());
	op->offset = offset;
	op->destination = destination;
	op->resolvedAddress = NULL;
	pointerVector.push_back(op);
}

std::vector<DMAMem::MemoryObject::ResolutionRequest> DMAMem::MemoryObject::getRequestedResolutions(QWORD baseAddress)
{
	return generateDefaultResolutions(baseAddress);
}

void DMAMem::MemoryObject::setUnresolved()
{
	this->_isBaseResolved = false;
	for (const auto ptrEntry : pointerVector) {
		ptrEntry->destination->setUnresolved();
	}
}

void DMAMem::MemoryObject::refreshObject(VmmManager* manager, DWORD pid)
{
	if (!_remoteAddress)
		return;
	this->setUnresolved();
	this->resolveObject(manager, pid, _remoteAddress);
}

std::vector<DMAMem::MemoryObject::ResolutionRequest> DMAMem::MemoryObject::generateDefaultResolutions(QWORD baseAddress)
{
	std::vector<ResolutionRequest> requestVec;;
	if (baseAddress == NULL)
		return requestVec;
	_remoteAddress = baseAddress;
	if (!_isBaseResolved) {
		for (const auto offEntry : offsetVector) {
			ResolutionRequest resReq;
			resReq.address = baseAddress + offEntry.offset;
			resReq.destination = offEntry.destination;
			resReq.size = offEntry.typeSize;
			requestVec.push_back(resReq);
		}
		for (const auto ptrEntry : pointerVector) {
			ResolutionRequest resReq;
			resReq.address = baseAddress + ptrEntry->offset;
			resReq.destination = &ptrEntry->resolvedAddress;
			resReq.size = GAME_POINTER_SIZE;
			requestVec.push_back(resReq);
		}
	}
	else {
		for (const auto ptrEntry : pointerVector) {
			auto ptrResolutions = ptrEntry->destination->getRequestedResolutions(ptrEntry->resolvedAddress);
			if (ptrResolutions.size() > 0) {
				DMAUtils::concatVectors<ResolutionRequest>(&requestVec, &ptrResolutions);
			}
		}
	}
	_isBaseResolved = true;
	return requestVec;
}

void DMAMem::MemoryObject::readResolutions(VmmManager* manager, DWORD pid, std::vector<ResolutionRequest> resolutionRequests, ULONG64 flags)
{
	VMMDLL_SCATTER_HANDLE scatterHandle = manager->initializeScatter(pid, flags);
	for (const auto res : resolutionRequests) {
		manager->addScatterRead(scatterHandle, res.address, res.size, res.destination);
	}
	manager->executeScatter(scatterHandle);
}



void DMAMem::MemoryObject::resolveObject(VmmManager* manager, DWORD pid, QWORD address, ULONG64 flags)
{
	auto resolutions = this->getRequestedResolutions(address);
	while (resolutions.size() > 0) {
		readResolutions(manager, pid, resolutions, flags);
		resolutions = this->getRequestedResolutions(address);
	}
}
