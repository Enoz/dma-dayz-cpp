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
			BOOL readMemory(DWORD pid, QWORD remoteAddress, void* destination, int size, ULONG64 flags = VMMDLL_FLAG_NOCACHE);
			template <typename T> T readMemoryDirect(DWORD pid, QWORD remoteAddress, ULONG64 flags = VMMDLL_FLAG_NOCACHE);
			VMMDLL_SCATTER_HANDLE initializeScatter(DWORD pid);
			VMMDLL_SCATTER_HANDLE initializeScatter(DWORD pid, ULONG64 flags);
			void addScatterRead(VMMDLL_SCATTER_HANDLE handle, QWORD remoteAddress, int size, void* destination);
			void executeScatter(VMMDLL_SCATTER_HANDLE handle);
	};

	template<typename T>
	inline T VmmManager::readMemoryDirect(DWORD pid, QWORD remoteAddress, ULONG64 flags)
	{
		T obj;
		VMMDLL_MemReadEx(this->getVmm(), pid, remoteAddress, (PBYTE)&obj, sizeof(T), NULL, flags);
		return obj;
	}
}
