#include "MemoryObjectScatterable.h"

void DMAMem::MemoryObjectScatterable::initializeScatter()
{
	scatterEntries.empty();
}

void DMAMem::MemoryObjectScatterable::registerScatterObject(MemoryObject* memObj, QWORD remoteAddress)
{
	ScatterEntry se;
	se.memObj = memObj;
	se.memObj->_remoteAddress = remoteAddress;
	se.remoteAddress = remoteAddress;
	scatterEntries.push_back(se);
}

void DMAMem::MemoryObjectScatterable::populateScatterObjects(VmmManager* vmmManager, DWORD remotePid, ULONG64 flags)
{
	VMMDLL_SCATTER_HANDLE scatterHandle = VMMDLL_Scatter_Initialize(vmmManager->getVmm(), remotePid, flags);
	auto resolvedList = std::vector<ResolvedScatterEntry>();
	for (ScatterEntry se : scatterEntries) {
		ResolvedScatterEntry rse;
		rse.se = se;
		int size = se.memObj->getObjectSize();
		rse.objectData = std::shared_ptr<char[]>(new char[size]);
		VMMDLL_Scatter_PrepareEx(scatterHandle, se.remoteAddress, size, (PBYTE)rse.objectData.get(), nullptr);
		resolvedList.push_back(rse);
	}
	VMMDLL_Scatter_ExecuteRead(scatterHandle);
	VMMDLL_Scatter_CloseHandle(scatterHandle);
	for (const ResolvedScatterEntry rse : resolvedList) {
		rse.se.memObj->resolveObject(vmmManager, remotePid, rse.objectData.get());
	}
	scatterEntries.empty();
}


