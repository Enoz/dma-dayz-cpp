#include "./VmmManager.h"

VMM_HANDLE DMAMem::VmmManager::getVmm() {
	if (!hVMM) {
		hVMM = initialize();
	}
	return hVMM;
}


VMM_HANDLE DMAMem::VmmManager::initialize()
{
	VMMDLL_CloseAll();
	LPSTR args[] = { LPSTR(""), LPSTR("-device"), LPSTR("fpga") };
	VMM_HANDLE handle = VMMDLL_Initialize(3, args);
	if (!handle)
		std::cout << "FAIL:    VMMDLL_Initialize\n" << std::endl;
	return handle;
}


