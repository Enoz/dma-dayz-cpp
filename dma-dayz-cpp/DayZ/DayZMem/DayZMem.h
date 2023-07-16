#pragma once
#include "../../DMAMemoryManagement/includes.h";
#define EXE_NAME "DayZ_x64.exe"
#include "../Structs/NetworkManager.h";
#include "../Structs/WorldPointer.h";

namespace DayZ {
	class Mem {
		DMAMem::VmmManager* vmmManager = NULL;
		DMAMem::StaticManager staticManager = NULL;
		QWORD worldAddress;
		QWORD networkManagerAddress;

	public:

		Mem(DMAMem::VmmManager* vmmManager);
		void fetchBaseAddresses();
		DWORD getPid();
		WorldPointer getWorld();
		NetworkManager getNetworkManager();
	};
};