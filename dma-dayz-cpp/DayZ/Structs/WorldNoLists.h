#pragma once
#include "../../DMAMemoryManagement/includes.h"

namespace DayZ {
	struct WorldNoLists : public DMAMem::MemoryObject {
		int32_t NearEntityTableCount;
		int32_t FarEntityTableCount;
		int32_t SlowEntityCountAlloc;
		int32_t SlowEntityValidCount;
		int32_t ItemTableCountAlloc;
		int32_t ItemTableCount;

		WorldNoLists() {
			this->registerOffset(0xEC0, &NearEntityTableCount, sizeof(int32_t));
			this->registerOffset(0x1008, &FarEntityTableCount, sizeof(int32_t));
			this->registerOffset(0x1F88, &SlowEntityCountAlloc, sizeof(int32_t));
			this->registerOffset(0x1F90, &SlowEntityValidCount, sizeof(int32_t));
			this->registerOffset(0x1FD8, &ItemTableCountAlloc, sizeof(int32_t));
			this->registerOffset(0x1FE0, &ItemTableCount, sizeof(int32_t));
		}
	};

}