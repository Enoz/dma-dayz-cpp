#pragma once
#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Entity.h";
#include "../DayzUtil.h";
#include <set>

class WeirdTable
{
public:
	uint16_t flag; //0x0000
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
		SlowItemStruct EntityPointers[512];
	public:
		std::vector<std::shared_ptr<Entity>> resolvedEntities;
		EntityTableSlowItem() {
			this->registerOffset(0x0, &EntityPointers, sizeof(SlowItemStruct[512]));
		}

		std::shared_ptr<std::vector<DMAMem::MemoryObject::ResolutionRequest>> getRequestedResolutions(QWORD baseAddress) override {
			if (!_isBaseResolved) {
				return generateDefaultResolutions(baseAddress);
			}
			if (resolvedEntities.size() > 0) {
				std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
				for (auto const ent : resolvedEntities) {
					DMAUtils::concatVectors(requestVec.get(), ent->getRequestedResolutions(ent->_lastAddressUsed).get());
				}
				return requestVec;
			}
			std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
			auto resolvedEntitiesTemp = std::vector<std::shared_ptr<Entity>>();
			for (auto entityPtr : EntityPointers) {
				if (DayZUtil::isPointerValid(entityPtr.EntityPointer)) {
					auto ent = std::shared_ptr<Entity>(new Entity());
					DMAUtils::concatVectors(requestVec.get(), ent->getRequestedResolutions(entityPtr.EntityPointer).get());
					resolvedEntities.push_back(ent);
				}
			}
			return requestVec;
		}
	};
}