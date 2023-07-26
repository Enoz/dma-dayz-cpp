#pragma once
#include "DMAMemoryManagement/includes.h";
#include "World.h";

namespace DayZ {
	struct WorldPointer : public DMAMem::MemoryObject {
		std::shared_ptr<World> WorldPtr;

		WorldPointer() {
			WorldPtr = std::shared_ptr<World>(new World());
			this->registerPointer(0x0, WorldPtr.get());
		}
	};
}