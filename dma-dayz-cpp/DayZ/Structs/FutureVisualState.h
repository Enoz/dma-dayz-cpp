#pragma once
#include "DMAMemoryManagement/includes.h";
#include "DMARender/includes.h"

namespace DayZ {
	struct FutureVisualState : public DMAMem::MemoryObject {
		DMARender::Vector3 position;

		FutureVisualState() {
			this->registerOffset(0x2c, &position, sizeof(DMARender::Vector3));
		}

	};
}