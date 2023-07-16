#include "DayZMem.h"

DMAMem::VmmManager* DayZ::Mem::getVMM()
{
	return vmmManager;
}

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
	std::cout << "World Address: 0x" << std::hex << this->worldAddress << std::endl;
	this->networkManagerAddress = baseModule.pvmEntry->vaBase + 0xEE7A88;
	std::cout << "NetworkManager Address: 0x" << std::hex << this->networkManagerAddress << std::endl;

};

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
