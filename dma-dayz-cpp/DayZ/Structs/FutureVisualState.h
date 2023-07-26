#pragma once
#include "DMAMemoryManagement/includes.h";
#include "Vectors.h";

namespace DayZ {
	struct FutureVisualState : public DMAMem::MemoryObject {
		Vector3 position;

		FutureVisualState() {
			this->registerOffset(0x2c, &position, sizeof(Vector3));
		}

	};
}