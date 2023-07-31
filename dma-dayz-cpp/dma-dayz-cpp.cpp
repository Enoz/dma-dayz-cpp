// dma-dayz-cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DMAMemoryManagement/includes.h"
#include "DayZ/DayZMem/DayZMem.h"
#include "DayZ/Structs/NetworkManager.h";
#include "DayZ/Structs/WorldPointer.h";
#include "DayZ/DayZMem/MemoryUpdater/MemoryUpdater.h"
#include "DayZ/DayZMem/OverlayAdapter/OverlayAdapter.h"
#include "DayZ/DayZMem/RadarAdapter/RadarAdapter.h"
#include "DMARender/includes.h"
#include "DayZ/Maps/ChernarusPlus.h"
#include "DayZ/Maps/Namalsk.h"

int main()
{


    auto vmm = new DMAMem::VmmManager();
    auto game = new DayZ::Mem(vmm);

    auto memUpdater = std::shared_ptr<DayZ::MemoryUpdater>(new DayZ::MemoryUpdater(game));
    memUpdater->beginUpdateLoop();

    DMARender::RenderWindow cmd = DMARender::RenderWindow();

    cmd.getBridge()->setOverlay(std::shared_ptr<DayZ::OverlayAdapter>(new DayZ::OverlayAdapter(memUpdater.get())));
    cmd.getBridge()->addMap(std::shared_ptr<DayZ::ChernarusPlus>(new DayZ::ChernarusPlus()));
    cmd.getBridge()->addMap(std::shared_ptr<DayZ::Namalsk>(new DayZ::Namalsk()));

    cmd.getBridge()->setRadar(std::shared_ptr<DayZ::RadarAdapter>(new DayZ::RadarAdapter(memUpdater.get())));

    std::thread gfxThread(&DMARender::RenderWindow::initializeWindow, &cmd);

    gfxThread.join();
    memUpdater->endUpdateLoop();

    return 1;
}
