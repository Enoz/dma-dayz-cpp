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


    auto vmm = new DMAMem::VmmManager();
    auto game = new DayZ::Mem(vmm);

    Overlay o(game);
    o.run();

    while (true) {}

    return 1;
}
