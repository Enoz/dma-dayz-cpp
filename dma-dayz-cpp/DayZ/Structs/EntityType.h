#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "ArmaString.h"

namespace DayZ {
	struct EntityType : public DMAMem::MemoryObject {
		std::shared_ptr<ArmaString> TypeName;
		std::shared_ptr<ArmaString> ModelName;
		std::shared_ptr<ArmaString> ConfigName;
		std::shared_ptr<ArmaString> CleanName;
		EntityType() {
			TypeName = std::shared_ptr<ArmaString>(new ArmaString());
			ModelName = std::shared_ptr<ArmaString>(new ArmaString());
			ConfigName = std::shared_ptr<ArmaString>(new ArmaString());
			CleanName = std::shared_ptr<ArmaString>(new ArmaString());
			this->registerPointer(0x70, TypeName.get());
			this->registerPointer(0x88, ModelName.get());
			this->registerPointer(0xA8, ConfigName.get());
			this->registerPointer(0x4E8, CleanName.get());
		}
	};
}