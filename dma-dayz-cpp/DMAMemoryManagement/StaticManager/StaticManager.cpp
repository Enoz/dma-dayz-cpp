#include "./StaticManager.h"



DWORD DMAMem::StaticManager::getPid(std::string processName, bool forceUpdate) {
	if (pidMap.find(processName) != pidMap.end() && !forceUpdate) {
		return pidMap[processName];
	}
	DWORD pid;
	VMMDLL_PidGetFromName(vmmManager->getVmm(), LPSTR(processName.c_str()), &pid);
	pidMap[processName] = pid;
	return pid;
}

DMAMem::MODULE_DUMP DMAMem::StaticManager::getModule(DWORD pid, std::string moduleName, bool forceUpdate) {
	auto pair = std::make_pair(pid, moduleName);
	if (dllMap.find(pair) != dllMap.end() && !forceUpdate) {
		return dllMap[pair];
	}
	PVMMDLL_MAP_MODULEENTRY pModuleEntry;
	VMMDLL_Map_GetModuleFromNameU(vmmManager->getVmm(), pid, LPSTR(moduleName.c_str()), &pModuleEntry, 0);
	DMAMem::MODULE_DUMP dumpEntry;
	dumpEntry.pvmEntry = pModuleEntry;
	dumpEntry.dump = std::shared_ptr<BYTE[]>(new BYTE[pModuleEntry->cbImageSize]);
	bool success = vmmManager->readMemory(pid, pModuleEntry->vaBase, dumpEntry.dump.get(), pModuleEntry->cbImageSize, VMMDLL_FLAG_NOCACHE);
	if (success) {
		std::cout << "[+] Dumped " << moduleName << std::endl;
	}
	else {
		std::cerr << "[-] Error Dumping " << moduleName << std::endl;
	}
	dllMap[pair] = dumpEntry;
	return dumpEntry;
}

DMAMem::StaticManager::StaticManager(VmmManager* manager)
{
	vmmManager = manager;
}

QWORD DMAMem::StaticManager::sigScan(DWORD pid, std::string moduleName, std::string pattern)
{
	//Read entire module
	MODULE_DUMP module = getModule(pid, moduleName);
	
	//Split pattern
	std::vector<std::string> patternSplit = DMAUtils::split(pattern, " ");
	std::vector<BYTE> patternBytes = std::vector<BYTE>();
	for (std::string sByte : patternSplit)
		if (sByte == "?")
			patternBytes.push_back((BYTE)0);
		else
			patternBytes.push_back((BYTE)std::stoul(sByte, nullptr, 16));

	for (int i = 0; i < module.pvmEntry->cbImageSize - patternSplit.size() - 1; i++) {
		bool failedSearch = false;
		for (int j = 0; j < patternSplit.size(); j++) {
			std::string patternString = patternSplit[j];
			if (patternString == "?")
				continue;
			BYTE moduleByte = module.dump[i + j];
			BYTE patternByte = patternBytes[j];
			if (moduleByte != patternByte) {
				failedSearch = true;
				break;
			}
		}
		if (!failedSearch) {
			return module.pvmEntry->vaBase + i;
		}
	}
	return NULL;
}

void DMAMem::StaticManager::sigScanWithRes(DWORD pid, std::string moduleName, std::string pattern, int offset, size_t length, PBYTE out) {
	QWORD sigScanRes = sigScan(pid, moduleName, pattern);
	VMMDLL_MemReadEx(vmmManager->getVmm(), pid, sigScanRes + offset, out, length, NULL, VMMDLL_FLAG_NOCACHE);
}

