#include "MemoryObject.h"

void DMAMem::MemoryObject::registerOffset(int offset, void* destination, int typeSize)
{
	OffsetEntry oe = OffsetEntry();
	oe.offset = offset;
	oe.destination = destination;
	oe.typeSize = typeSize;
	offsetVector->push_back(oe);
}

void DMAMem::MemoryObject::registerPointer(int offset, MemoryObject* destination, ULONG64 flags)
{
	std::shared_ptr<OffsetPointer> op(new OffsetPointer());
	op->offset = offset;
	op->destination = destination;
	op->flags = flags;
	op->resolvedAddress = NULL;
	pointerVector->push_back(op);
}

std::vector<DMAMem::MemoryObject::ResolutionRequest>* DMAMem::MemoryObject::getRequestedResolutions(QWORD baseAddress)
{
	return generateDefaultResolutions(baseAddress);
}

std::vector<DMAMem::MemoryObject::ResolutionRequest>* DMAMem::MemoryObject::generateDefaultResolutions(QWORD baseAddress)
{
	std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
	if (baseAddress == NULL)
		return requestVec.get();
	_lastAddressUsed = baseAddress;
	if (!_isBaseResolved) {
		for (const auto offEntry : *offsetVector) {
			ResolutionRequest resReq;
			resReq.address = baseAddress + offEntry.offset;
			resReq.destination = offEntry.destination;
			resReq.size = offEntry.typeSize;
			requestVec->push_back(resReq);
		}
		for (const auto ptrEntry : *pointerVector) {
			ResolutionRequest resReq;
			resReq.address = baseAddress + ptrEntry->offset;
			resReq.destination = &ptrEntry->resolvedAddress;
			resReq.size = GAME_POINTER_SIZE;
			requestVec->push_back(resReq);
		}
	}
	else {
		for (const auto ptrEntry : *pointerVector) {
			auto ptrResolutions = ptrEntry->destination->getRequestedResolutions(ptrEntry->resolvedAddress);
			if (ptrResolutions->size() > 0) {
				DMAUtils::concatVectors<ResolutionRequest>(requestVec.get(), ptrResolutions);
			}
		}
	}
	_isBaseResolved = true;
	return requestVec.get();
}

void DMAMem::MemoryObject::readResolutions(VmmManager* manager, DWORD pid, std::vector<ResolutionRequest>* resolutionRequests)
{
	VMMDLL_SCATTER_HANDLE scatterHandle = manager->initializeScatter(pid);
	for (const auto res : *resolutionRequests) {
		manager->addScatterRead(scatterHandle, res.address, res.size, res.destination);
	}
	manager->executeScatter(scatterHandle);
}



void DMAMem::MemoryObject::resolveObject(VmmManager* manager, DWORD pid, QWORD address)
{
	auto resolutions = this->getRequestedResolutions(address);
	while (resolutions->size() > 0) {
		readResolutions(manager, pid, resolutions);
		auto resolutions = this->getRequestedResolutions(address);
	}
}
