#include "MemoryObject.h"

void DMAMem::MemoryObject::registerOffset(int offset, void* destination, int typeSize)
{
	OffsetEntry oe = OffsetEntry();
	oe.offset = offset;
	oe.destination = destination;
	oe.typeSize = typeSize;
	offsetVector.push_back(oe);
}

void DMAMem::MemoryObject::registerPointer(int offset, MemoryObject* destination, ULONG64 flags)
{
	OffsetPointer op = OffsetPointer();
	op.offset = offset;
	op.destination = destination;
	op.flags = flags;
	pointerVector.push_back(op);
}

int DMAMem::MemoryObject::getObjectSize()
{
	int memReadSize = 0;
	for (const OffsetEntry& oe : offsetVector) {
		memReadSize = (((memReadSize) > (oe.offset + oe.typeSize)) ? (memReadSize) : (oe.offset + oe.typeSize));
	}
	for (const OffsetPointer& op : pointerVector) {
		memReadSize = (((memReadSize) > (op.offset + GAME_POINTER_SIZE)) ? (memReadSize) : (op.offset + GAME_POINTER_SIZE));
	}
	return memReadSize;
}



BOOL DMAMem::MemoryObject::resolveObject(VmmManager* vmmManager, DWORD remotePid, char* objectData) {
	for (const OffsetEntry& oe : offsetVector) {
		memcpy(oe.destination, objectData + oe.offset, oe.typeSize);
	}
	for (const OffsetPointer& op : pointerVector) {
		GAME_POINTER_TYPE gp;
		memcpy(&gp, objectData + op.offset, GAME_POINTER_SIZE);
		op.destination->resolveOffsets(vmmManager, remotePid, gp, op.flags);
		op.destination->postPointerResolution(vmmManager, remotePid);
	}
	return TRUE;
}

BOOL DMAMem::MemoryObject::resolveOffsets(VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress, ULONG64 flags)
{
	if (remoteAddress == NULL) {
		return FALSE;
	}
	_remoteAddress = remoteAddress;
	int memReadSize = getObjectSize();
	std::shared_ptr<char[]> objectData(new char[memReadSize]);

	if (vmmManager->readMemory(remotePid, remoteAddress, objectData.get(), memReadSize, flags)) {
		return resolveObject(vmmManager, remotePid, objectData.get());
	}
	return FALSE;
}

void DMAMem::MemoryObject::initializeScatter()
{
	scatterEntries.empty();
}

void DMAMem::MemoryObject::registerScatterObject(MemoryObject* memObj, QWORD remoteAddress)
{
	ScatterEntry se;
	se.memObj = memObj;
	se.memObj->_remoteAddress = remoteAddress;
	se.remoteAddress = remoteAddress;
	scatterEntries.push_back(se);
}

void DMAMem::MemoryObject::populateScatterObjects(VmmManager* vmmManager, DWORD remotePid, ULONG64 flags)
{
	VMMDLL_SCATTER_HANDLE scatterHandle = vmmManager->initializeScatter(remotePid, flags);

	auto resolvedList = std::vector<ResolvedScatterEntry>();
	for (ScatterEntry se : scatterEntries) {
		ResolvedScatterEntry rse;
		rse.se = se;
		int size = se.memObj->getObjectSize();
		rse.objectData = std::shared_ptr<char[]>(new char[size]);
		vmmManager->addScatterRead(scatterHandle, se.remoteAddress, size, rse.objectData.get());
		resolvedList.push_back(rse);
	}
	vmmManager->executeScatter(scatterHandle);
	for (const ResolvedScatterEntry rse : resolvedList) {
		rse.se.memObj->resolveObject(vmmManager, remotePid, rse.objectData.get());
	}
	scatterEntries.empty();
}