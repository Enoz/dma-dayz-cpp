#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Entity.h";
#include "../DayzUtil.h";
#include <set>

namespace DayZ {
	class EntityTable : public DMAMem::MemoryObject {
		QWORD EntityPointers[512];
	public:
		std::vector<std::shared_ptr<Entity>> resolvedEntities;
		EntityTable() {
			this->registerOffset(0x0, &EntityPointers, sizeof(QWORD[256]));
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
			std::set<QWORD> vecEntityPointers(std::begin(EntityPointers), std::end(EntityPointers));
			for (QWORD entityPtr : vecEntityPointers) {
				if (DayZUtil::isPointerValid(entityPtr)) {
					auto ent = std::shared_ptr<Entity>(new Entity());
					auto entRes = ent->getRequestedResolutions(entityPtr);
					DMAUtils::concatVectors(&requestVec, &entRes);
					resolvedEntities.push_back(ent);
				}
			}
			return requestVec;
		}
	};
}