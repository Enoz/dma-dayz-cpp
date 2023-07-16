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
	OffsetPointer op = OffsetPointer();
	op.offset = offset;
	op.destination = destination;
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
		op.destination->resolveOffsets(vmmManager, remotePid, gp);
	}
	return TRUE;
}

BOOL DMAMem::MemoryObject::resolveOffsets(VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress)
{
	if (remoteAddress == NULL) {
		return FALSE;
	}
	int memReadSize = getObjectSize();
	std::shared_ptr<char[]> objectData(new char[memReadSize]);


	if (VMMDLL_MemReadEx(vmmManager->getVmm(), remotePid, remoteAddress, (PBYTE)objectData.get(), memReadSize, NULL, VMMDLL_FLAG_NOCACHE)) {
		return resolveObject(vmmManager, remotePid, objectData.get());
	}
	return FALSE;
}
