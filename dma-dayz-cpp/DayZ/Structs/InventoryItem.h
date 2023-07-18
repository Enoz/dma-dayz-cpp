#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "FutureVisualState.h";
#include "EntityType.h"
#include "Scoreboard.h"
#include "ScoreboardIdentity.h"
#include "EntityInventory.h"


namespace DayZ {

	class InventoryItem : public DMAMem::MemoryObject {
	public:

		std::shared_ptr<EntityType> EntityTypePtr;


		InventoryItem() {
			EntityTypePtr = std::shared_ptr<EntityType>(new EntityType());
			this->registerPointer(0x168, EntityTypePtr.get());
		}

	};
}