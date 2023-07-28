// dma-dayz-cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DMAMemoryManagement/includes.h"
#include "DayZ/DayZMem/DayZMem.h"
#include "DayZ/Structs/NetworkManager.h";
#include "DayZ/Structs/WorldPointer.h";
#include "DayZ/DayZMem/MemoryUpdater/MemoryUpdater.h"
#include "DMARender/includes.h"
#include "DayZ/DayZMem/OverlayAdapter/OverlayAdapter.h"

int main()
{


    auto vmm = new DMAMem::VmmManager();
    auto game = new DayZ::Mem(vmm);

    auto memUpdater = std::shared_ptr<DayZ::MemoryUpdater>(new DayZ::MemoryUpdater(game));
    memUpdater->beginUpdateLoop();

    DMARender::Commander cmd = DMARender::Commander();

    std::shared_ptr<DayZ::OverlayAdapter> overlayAdapter(new DayZ::OverlayAdapter(memUpdater.get()));
    cmd.getAdapter()->setOverlay(overlayAdapter);

    std::thread gfxThread(&DMARender::Commander::initializeWindow, &cmd);

    gfxThread.join();
    memUpdater->endUpdateLoop();

    return 1;
}
