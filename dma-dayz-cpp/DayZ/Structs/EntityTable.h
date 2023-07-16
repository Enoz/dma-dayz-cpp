#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Entity.h";
#include "../DayzUtil.h";
#include <set>

namespace DayZ {
	class EntityTable : public DMAMem::MemoryObject {
		QWORD EntityPointers[256];
	public:
		std::vector<std::shared_ptr<Entity>> resolvedEntities;
		EntityTable() {
			this->registerOffset(0x0, &EntityPointers, sizeof(QWORD[256]));
		}

		//std::vector<ResolutionRequest>* getRequestedResolutions(QWORD baseAddress) override {
		//	if (!_isBaseResolved) {
		//		return generateDefaultResolutions(baseAddress);
		//	}
		//	if (resolvedEntities.size() > 0) {
		//		std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
		//		for (auto const ent : resolvedEntities) {
		//			DMAMem::Utils::concatVectors(requestVec.get(), ent->getRequestedResolutions(ent->_lastAddressUsed));
		//		}
		//		return requestVec.get();
		//	}
		//	std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
		//	auto resolvedEntitiesTemp = std::vector<std::shared_ptr<Entity>>();
		//	std::set<QWORD> vecEntityPointers(std::begin(EntityPointers), std::end(EntityPointers));
		//	for (QWORD entityPtr : vecEntityPointers) {
		//		if (DzUtil::isPointerValid(entityPtr)) {
		//			auto ent = std::shared_ptr<Entity>(new Entity());
		//			DMAMem::Utils::concatVectors(requestVec.get(), ent->getRequestedResolutions(entityPtr));
		//			resolvedEntities.push_back(ent);
		//		}
		//	}
		//}
	};
}