#pragma once
#include "DMAMemoryManagement/includes.h";
#include "FutureVisualState.h";
#include "EntityType.h"
#include "Scoreboard.h"
#include "ScoreboardIdentity.h"
#include "EntityInventory.h"
#include "EntityFilterList.h"


namespace DayZ {


	enum ENTITY_TYPE {
		NONE,
		GROUNDITEM,
		PLAYER,
		ZOMBIE,
		ANIMAL
	};

	class Entity : public DMAMem::MemoryObject {

		ENTITY_TYPE _entityType = ENTITY_TYPE::NONE;
		bool _isEntityTypeChecked = false;
		bool _isValid = false;
		bool _isValidChecked = false;

	public:

		void categorizeEntity() {
			if (_entityType == ENTITY_TYPE::NONE) {
				if (!strcmp("dayzplayer", this->EntityTypePtr->ConfigName->value)) {
					_entityType = ENTITY_TYPE::PLAYER;
					return;
				}
				if (!strcmp("dayzinfected", this->EntityTypePtr->ConfigName->value)) {
					_entityType = ENTITY_TYPE::ZOMBIE;
					return;
				}
				if (!strcmp("dayzanimal", this->EntityTypePtr->ConfigName->value)) {
					_entityType = ENTITY_TYPE::ANIMAL;
					return;
				}
				for (auto const whiteListEntry : InventoryItemWhitelist) {
					if (!strcmp(whiteListEntry.c_str(), this->EntityTypePtr->ConfigName->value)) {
						_entityType = ENTITY_TYPE::GROUNDITEM;
						return;
					}
				}
			}
		}

		ENTITY_TYPE getEntityType() {
			if (!_isEntityTypeChecked) {
				categorizeEntity();
				_isEntityTypeChecked = true;
			}
			return _entityType;
		}

		std::shared_ptr<FutureVisualState> FutureVisualStatePtr;
		std::shared_ptr<EntityType> EntityTypePtr;
		std::shared_ptr<EntityInventory> InventoryPtr;
		uint32_t NetworkID;
		bool isDead;
		

		Entity() {
			FutureVisualStatePtr = std::shared_ptr<FutureVisualState>(new FutureVisualState());
			EntityTypePtr = std::shared_ptr<EntityType>(new EntityType());
			InventoryPtr = std::shared_ptr<EntityInventory>(new EntityInventory());
			this->registerPointer(0x180, EntityTypePtr.get());
			this->registerPointer(0x1D0, FutureVisualStatePtr.get());
			this->registerPointer(0x680, InventoryPtr.get());
			this->registerOffset(0x70C, &NetworkID, sizeof(uint32_t));
			this->registerOffset(0xE2, &isDead, sizeof(bool));
		}

		

		bool isValid() {

			if (!_isValidChecked) {
				for (auto blacklistItem : itemBlacklist) {
					if (!strcmp(blacklistItem.c_str(), this->EntityTypePtr->TypeName->value)) {
						_isValid = false;
						_isValidChecked = true;
						return _isValid;
					}
				}
				_isValid = this->EntityTypePtr->TypeName->length < 400 && this->EntityTypePtr->TypeName->length > 0;
				_isValidChecked = true;
			}
			return _isValid;
		}

		bool isPlayer() {
			return getEntityType() == PLAYER;
		}

		bool isZombie() {
			return getEntityType() == ZOMBIE;
		}

		bool isAnimal() {
			return getEntityType() == ANIMAL;
		}

		bool isGroundItem() {
			return getEntityType() == GROUNDITEM;
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