#pragma once
#include "DMAMemoryManagement/includes.h"

namespace DayZ {
	struct WorldNoLists : public DMAMem::MemoryObject {
		int32_t NearEntityTableCount;
		int32_t FarEntityTableCount;
		int32_t SlowEntityCountAlloc;
		int32_t SlowEntityValidCount;
		int32_t ItemTableCountAlloc;
		int32_t ItemTableCount;

		QWORD NearTableAddress;
		QWORD FarTableAddress;
		QWORD ItemTableAddress;
		QWORD SlowTableAddress;

		WorldNoLists() {
			this->registerOffset(0xF48 + 0x8, &NearEntityTableCount, sizeof(int32_t));
			this->registerOffset(0x1090 + 0x8, &FarEntityTableCount, sizeof(int32_t));
			this->registerOffset(0x2010 + 0x8, &SlowEntityCountAlloc, sizeof(int32_t));
			this->registerOffset(0x1F90 + 0x8 + 0x8, &SlowEntityValidCount, sizeof(int32_t));
			this->registerOffset(0x2060 + 0x8, &ItemTableCountAlloc, sizeof(int32_t));
			this->registerOffset(0x2060 + 0x8 + 0x8, &ItemTableCount, sizeof(int32_t));
			/*
							this->registerPointer(0xEC8, NearEntityTable.get());
				this->registerPointer(0x1010, FarEntityTable.get());
				this->registerPointer(0x1F90, SlowEntityTable.get());
				this->registerPointer(0x1FE0, ItemTable.get());
			*/
			this->registerOffset(0xF48, &NearTableAddress, sizeof(QWORD));
			this->registerOffset(0x1090, & FarTableAddress, sizeof(QWORD));
			this->registerOffset(0x2010, &SlowTableAddress, sizeof(QWORD));
			this->registerOffset(0x2060, &ItemTableAddress, sizeof(QWORD));

		}
	};

}