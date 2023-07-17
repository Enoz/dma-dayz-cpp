#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "FutureVisualState.h";
#include "EntityType.h"


namespace DayZ {

	const std::vector<std::string> InventoryItemWhitelist {"inventoryItem", "item", "clothing", "ProxyMagazines", "Weapon", "itemoptics"};

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

		

		bool isValid() {
			return this->EntityTypePtr->TypeName->length < 400 && this->EntityTypePtr->TypeName->length > 0;
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