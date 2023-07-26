#pragma once
#include "DMAMemoryManagement/includes.h";

namespace DayZ {
	struct ArmaString : public DMAMem::MemoryObject {
		uint16_t length;
		char value[256];

		ArmaString() {
			this->registerOffset(0x8, &length, sizeof(uint16_t));
			this->registerOffset(0x10, &value, sizeof(char[256]));
		}
	};
}