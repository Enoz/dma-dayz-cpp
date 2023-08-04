#pragma once
#pragma once
#include "DMAMemoryManagement/includes.h";
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
		std::shared_ptr<SlowItemStruct[]> EntityPointers;
		int allocSize;
		int validSize;
	protected:
		std::vector<DMAMem::MemoryObject::ResolutionRequest> postResolveResolutions() override {
			if (resolvedEntities.size() > 0) {
				std::vector<ResolutionRequest> requestVec;
				for (auto const ent : resolvedEntities) {
					auto entRes = ent->getRequestedResolutions(ent->_remoteAddress);
					DMAUtils::concatVectors(&requestVec, &entRes);
				}
				return requestVec;
			}

			std::vector<ResolutionRequest> requestVec;

			for (int i = 0; i < allocSize; i++) {
				SlowItemStruct entityPtr = EntityPointers[i];
				if (entityPtr.flag == 1) {
					auto ent = std::shared_ptr<Entity>(new Entity());
					auto entRes = ent->getRequestedResolutions(entityPtr.EntityPointer);
					DMAUtils::concatVectors(&requestVec, &entRes);
					resolvedEntities.push_back(ent);
					if (resolvedEntities.size() == validSize)
						break;
				}
			}
			return requestVec;
		}

	public:
		std::vector<std::shared_ptr<Entity>> resolvedEntities;
		EntityTableSlowItem(int allocSize, int validSize) {
			this->validSize = validSize;
			this->allocSize = allocSize;
			EntityPointers = std::shared_ptr<SlowItemStruct[]>(new SlowItemStruct[allocSize]);
			this->registerOffset(0x0, EntityPointers.get(), sizeof(SlowItemStruct) * allocSize);
		}
	};
}