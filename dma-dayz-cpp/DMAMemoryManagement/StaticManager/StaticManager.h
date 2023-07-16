#pragma once
#include <Windows.h>
#include "../VmmManager/VmmManager.h"
#include "../Utils/Utils.h"
#include <string>
#include <map>
#include <vector>



namespace DMAMem {
	struct MODULE_DUMP {
		PVMMDLL_MAP_MODULEENTRY pvmEntry;
		std::shared_ptr<BYTE[]> dump;
	};

	class StaticManager {
	private:
		VmmManager* vmmManager;
		std::map<std::string, DWORD> pidMap;
		std::map < std::pair<DWORD, std::string>, MODULE_DUMP > dllMap;
	public:
		StaticManager(VmmManager* manager);
		DWORD getPid(std::string processName, bool forceUpdate = false);
		MODULE_DUMP getModule(DWORD pid, std::string moduleName, bool forceUpdate = false);
		QWORD sigScan(DWORD pid, std::string moduleName, std::string pattern);

		void sigScanWithRes(DWORD pid, std::string moduleName, std::string pattern, int offset, size_t length, PBYTE out);
	};
}