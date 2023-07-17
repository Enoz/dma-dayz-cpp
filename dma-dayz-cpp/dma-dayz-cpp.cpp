// dma-dayz-cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "DayZ/Overlay/Overlay.h";
#include <iostream>
#include "DMAMemoryManagement/includes.h"
#include "DayZ/DayZMem/DayZMem.h"
#include "DayZ/Structs/NetworkManager.h";
#include "DayZ/Structs/WorldPointer.h";

int main()
{
    //std::cout << "Hello World!\n";
    //auto vmm = new DMAMem::VmmManager();
    //auto staticManager = new DMAMem::StaticManager(vmm);
    //DWORD pid = staticManager->getPid("DayZ_x64.exe");
    //auto baseModule = staticManager->getModule(pid, "DayZ_x64.exe");
    //std::cout << "Base At " << std::hex << baseModule.pvmEntry->vaBase << std::endl;
    //std::cout << "NetworkManager At " << std::hex << baseModule.pvmEntry->vaBase + 0xee7a88 << std::endl;
    //auto nm = DayZ::NetworkManager(vmm, pid, baseModule.pvmEntry->vaBase + 0xee7a88);
    //std::cout << "Player Count" << std::dec << nm.NetworkClientPtr->PlayerCount << std::endl;
    //for (const auto ident : nm.NetworkClientPtr->scoreboardPtr->resolvedIdentities) {
    //    std::cout << ident->PlayerName->value << ": " << ident->NetworkID << std::endl;
    //}

    //auto wrld = DayZ::WorldPointer(vmm, pid, baseModule.pvmEntry->vaBase + 0x413B418);
    //for (auto const ent : wrld.WorldPtr->SlowEntityTable->resolvedEntities) {
    //    std::cout << ent->EntityTypePtr->ConfigName->value << " -- " << ent->NetworkID << std::endl;
    //}
    //for (auto const ent : wrld.WorldPtr->FarEntityTable->resolvedEntities) {
    //    std::cout << ent->EntityTypePtr->ConfigName->value << " -- " << ent->NetworkID << std::endl;
    //}
    //for (auto const ent : wrld.WorldPtr->NearEntityTable->resolvedEntities) {
    //    std::cout << ent->EntityTypePtr->ConfigName->value << " -- " << ent->NetworkID << std::endl;
    //}
    //for (auto const ent : wrld.WorldPtr->ItemTable->resolvedEntities) {
    //    std::cout << ent->EntityTypePtr->ConfigName->value << " -- " << ent->NetworkID << std::endl;
    //}

    //std::cout << "Done!" << std::endl;

    auto vmm = new DMAMem::VmmManager();
    auto game = new DayZ::Mem(vmm);

    //auto wrld = game->getWorld();
    //std::cout << "Success?" << std::endl;
    //auto nm = game->getNetworkManager();
    //for (const auto id : nm.NetworkClientPtr->scoreboardPtr->resolvedIdentities) {
    //    std::cout << id->PlayerName->value << std::endl;
    //}


    ////auto world = game->getWorld();
    ////auto nm = game->getNetworkManager();
    ////
    Overlay o(game);
    o.run();

    while (true) {}

    return 1;
}
