// dma-dayz-cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DMAMemoryManagement/includes.h"

int main()
{
    std::cout << "Hello World!\n";
    auto manager = new DMAMem::VmmManager();
    manager->getVmm();
}
