#pragma once
#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Entity.h";
#include "../DayzUtil.h";
#include <set>

class WeirdTable
{
public:
	uint16_t validItemFlag; //0x0000
	char pad_0002[6]; //0x0002
	QWORD WeirdPtr; //0x0008
	QWORD Junk; //0x0010
}; //Size: 0x0018

namespace DayZ {
	class EntityTableSlowItem : public DMAMem::MemoryObject {
		struct SlowItemStruct {
			uint16_t flag; //0x0000
			char pad_0002[6]; //0x0002
			QWORD EntityPointer; //0x0008
			QWORD Junk; //0x0010
		};
		SlowItemStruct EntityPointers[256];
	public:
		std::vector<std::shared_ptr<Entity>> resolvedEntities;
		EntityTableSlowItem() {
			this->registerOffset(0x0, &EntityPointers, sizeof(SlowItemStruct[256]));
		}

		std::vector<DMAMem::MemoryObject::ResolutionRequest> getRequestedResolutions(QWORD baseAddress) override {
			if (!_isBaseResolved) {
				return generateDefaultResolutions(baseAddress);
			}
			if (resolvedEntities.size() > 0) {
				std::vector<ResolutionRequest> requestVec;
				for (auto const ent : resolvedEntities) {
					auto entRes = ent->getRequestedResolutions(ent->_remoteAddress);
					DMAUtils::concatVectors(&requestVec, &entRes);
				}
				return requestVec;
			}
			std::vector<ResolutionRequest> requestVec;
			auto resolvedEntitiesTemp = std::vector<std::shared_ptr<Entity>>();
			for (const auto entityPtr : EntityPointers) {
				if (DayZUtil::isPointerValid(entityPtr.EntityPointer)) {
					auto ent = std::shared_ptr<Entity>(new Entity());
					auto entRes = ent->getRequestedResolutions(entityPtr.EntityPointer);
					DMAUtils::concatVectors(&requestVec, &entRes);
					resolvedEntities.push_back(ent);
				}
			}
			return requestVec;
		}
	};
}