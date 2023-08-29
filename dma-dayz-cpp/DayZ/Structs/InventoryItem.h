#pragma once
#include "DMAMemoryManagement/includes.h";
#include "FutureVisualState.h";
#include "EntityType.h"
#include "Scoreboard.h"
#include "ScoreboardIdentity.h"
#include "EntityInventory.h"
#include "EntityFilterList.h"


namespace DayZ {


	class InventoryItem : public DMAMem::MemoryObject {

		bool _isValid = false;
		bool _isValidChecked = false;

	public:

		std::shared_ptr<EntityType> EntityTypePtr;


		InventoryItem() {
			EntityTypePtr = std::shared_ptr<EntityType>(new EntityType());
			this->registerPointer(0x180, EntityTypePtr.get());
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

	};
}