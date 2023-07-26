#pragma once
#include "DMAMemoryManagement/includes.h";
#define EXE_NAME "DayZ_x64.exe"
#include "../Structs/NetworkManager.h";
#include "../Structs/WorldPointer.h";

namespace DayZ {
	class Mem {
		DMAMem::VmmManager* vmmManager = NULL;
		DMAMem::StaticManager staticManager = NULL;
		QWORD worldAddress;
		QWORD networkManagerAddress;
		void fetchBaseAddresses();

	public:
		Mem(DMAMem::VmmManager* vmmManager);
		DMAMem::VmmManager* getVMM();
		DWORD getPid();
		WorldPointer getWorld();
		NetworkManager getNetworkManager();
		std::vector<std::shared_ptr<DayZ::Entity>> getAllUniqueEntities();
	};
};