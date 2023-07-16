#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "FutureVisualState.h";
#include "EntityType.h"

namespace DayZ {
	struct Entity : public DMAMem::MemoryObject {
		std::shared_ptr<FutureVisualState> FutureVisualStatePtr;
		std::shared_ptr<EntityType> EntityTypePtr;
		uint32_t NetworkID;
		

		Entity() {
			FutureVisualStatePtr = std::shared_ptr<FutureVisualState>(new FutureVisualState());
			EntityTypePtr = std::shared_ptr<EntityType>(new EntityType());
			this->registerPointer(0x168, EntityTypePtr.get());
			this->registerPointer(0x1C0, FutureVisualStatePtr.get());
			this->registerOffset(0x6DC, &NetworkID, sizeof(uint32_t));
		}
	};
}