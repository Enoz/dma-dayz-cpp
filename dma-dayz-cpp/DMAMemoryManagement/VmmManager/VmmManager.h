#pragma once
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "../MemProcFS//leechcore.h"
#include "../MemProcFS/vmmdll.h"
#pragma comment(lib, "leechcore")
#pragma comment(lib, "vmm")

namespace DMAMem {
	class VmmManager {
	private:
			VMM_HANDLE hVMM;
			VMM_HANDLE initialize();
	public:
			VMM_HANDLE getVmm();

	};
}
