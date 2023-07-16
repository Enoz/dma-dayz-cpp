#include "DayZMem.h"

DayZ::Mem::Mem(DMAMem::VmmManager* vmmManager)
{
	this->staticManager = DMAMem::StaticManager(vmmManager);
	this->vmmManager = vmmManager;
	fetchBaseAddresses();
}

void DayZ::Mem::fetchBaseAddresses() {
	auto pid = getPid();
	auto baseModule = staticManager.getModule(pid, EXE_NAME);

	this->worldAddress = baseModule.pvmEntry->vaBase + 0x413B418;
	this->networkManagerAddress = baseModule.pvmEntry->vaBase + 0xEE7A88;
}

DWORD DayZ::Mem::getPid() {
	return staticManager.getPid(EXE_NAME);
}

DayZ::WorldPointer DayZ::Mem::getWorld()
{
	return WorldPointer(vmmManager, getPid(), this->worldAddress);
}

DayZ::NetworkManager DayZ::Mem::getNetworkManager()
{
	return NetworkManager(vmmManager, getPid(), this->networkManagerAddress);
}
