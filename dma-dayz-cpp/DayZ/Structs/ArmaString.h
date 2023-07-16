#pragma once
#include "../../DMAMemoryManagement/includes.h";

namespace DayZ {
	class ArmaString : public DMAMem::MemoryObject {

	public:

		uint16_t length;
		char value[256];

		ArmaString() {
			this->registerOffset(0x0, &length, sizeof(uint16_t));
			this->registerOffset(0x10, &value, sizeof(char[256]));
		}

		ArmaString(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : ArmaString() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
		}
	};
}