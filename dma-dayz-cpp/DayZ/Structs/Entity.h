#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "FutureVisualState.h";
#include "EntityType.h"


namespace DayZ {
	struct Entity : public DMAMem::MemoryObject {
		std::shared_ptr<FutureVisualState> FutureVisualStatePtr;
		std::shared_ptr<EntityType> EntityTypePtr;
		uint32_t NetworkID;
		bool isDead;
		

		Entity() {
			FutureVisualStatePtr = std::shared_ptr<FutureVisualState>(new FutureVisualState());
			EntityTypePtr = std::shared_ptr<EntityType>(new EntityType());
			this->registerPointer(0x168, EntityTypePtr.get());
			this->registerPointer(0x1C0, FutureVisualStatePtr.get());
			this->registerOffset(0x6DC, &NetworkID, sizeof(uint32_t));
			this->registerOffset(0x17D, &isDead, sizeof(bool));
		}

		bool isPlayer() {
			return !strcmp("dayzplayer", this->EntityTypePtr->ConfigName->value);
		}

		bool isZombie() {
			return !strcmp("dayzinfected", this->EntityTypePtr->ConfigName->value);
		}

		bool isAnimal() {
			return !strcmp("dayzanimal", this->EntityTypePtr->ConfigName->value);
		}

		bool isInventoryItem() {
			return !strcmp("inventoryitem", this->EntityTypePtr->ConfigName->value);
		}

		std::shared_ptr<ScoreboardIdentity> getPlayerIdentity(DayZ::Scoreboard* scoreboard) {
			for (const auto ident : scoreboard->resolvedIdentities) {
				if (ident->NetworkID == this->NetworkID)
					return ident;
			}
			return NULL;
		}

	};
}