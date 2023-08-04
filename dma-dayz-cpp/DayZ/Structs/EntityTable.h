#pragma once
#include "DMAMemoryManagement/includes.h";
#include "Entity.h";
#include "../DayzUtil.h";
#include <set>

namespace DayZ {
	class EntityTable : public DMAMem::MemoryObject {
		//QWORD EntityPointers[256];
		std::shared_ptr<QWORD[]> EntityPointers;
		int tableSize;
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
			for (int i = 0; i < tableSize; i++) {
				QWORD entityPtr = EntityPointers[i];
				auto ent = std::shared_ptr<Entity>(new Entity());
				auto entRes = ent->getRequestedResolutions(entityPtr);
				DMAUtils::concatVectors(&requestVec, &entRes);
				resolvedEntities.push_back(ent);
			}

			return requestVec;
		}
	public:
		std::vector<std::shared_ptr<Entity>> resolvedEntities;
		EntityTable(int size) {
			tableSize = size;
			EntityPointers = std::shared_ptr<QWORD[]>(new QWORD[tableSize]);
			this->registerOffset(0x0, EntityPointers.get(), sizeof(QWORD) * tableSize);
		}
	};
}