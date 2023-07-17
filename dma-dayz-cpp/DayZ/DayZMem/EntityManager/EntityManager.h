#pragma once
#include "../../../DMAMemoryManagement/MemoryObject/MemoryObject.h"
#include "../../Structs/Entity.h"

namespace DayZ {
	class EntityManager : public DMAMem::MemoryObject {
		std::vector<std::shared_ptr<DayZ::Entity>> groundItems;
		std::vector<std::shared_ptr<DayZ::Entity>> characters;
		std::vector<std::shared_ptr<DayZ::Entity>> uncategorized;
		DMAMem::VmmManager* manager;
		DWORD pid;
	public:
		EntityManager(DMAMem::VmmManager* manager, DWORD pid, std::shared_ptr<std::vector<std::shared_ptr<DayZ::Entity>>> ents) {
			this->manager = manager;
			this->pid = pid;
			for (auto const ent : *ents) {

				if (ent->isPlayer() || ent->isAnimal() || ent->isZombie()) {
					characters.push_back(ent);
					continue;
				}

				if (ent->isGroundItem()) {
					groundItems.push_back(ent);
					continue;
				}

				uncategorized.push_back(ent);
			}
		}

		std::vector<std::shared_ptr<DayZ::Entity>> getGroundItems() {
			return groundItems;
		}

		std::vector<std::shared_ptr<DayZ::Entity>> getCharacters() {
			return characters;
		}

		std::vector<std::shared_ptr<DayZ::Entity>> getUncategorized() {
			return uncategorized;
		}

		void refreshGroundItems() {
			for (const auto ent : groundItems) {
				ent->setUnresolved();
			}
			this->resolveObject(manager, pid, 1);
		}

		void refreshCharacters() {
			for (const auto ent : characters) {
				ent->setUnresolved();
			}
			this->resolveObject(manager, pid, 1);
		}

		void refreshUncategorized() {
			for (const auto ent : uncategorized) {
				ent->setUnresolved();
			}
			this->resolveObject(manager, pid, 1);
		}


		std::shared_ptr<std::vector<DMAMem::MemoryObject::ResolutionRequest>> getRequestedResolutions(QWORD baseAddress) override {
			std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
			for (const auto ent : groundItems) {
				auto resolutions = *ent->getRequestedResolutions(ent->_remoteAddress);
				for (const auto& resReq : resolutions) {
					requestVec->push_back(resReq);
				}
			}
			for (const auto ent : characters) {
				auto resolutions = *ent->getRequestedResolutions(ent->_remoteAddress);
				for (const auto& resReq : resolutions) {
					requestVec->push_back(resReq);
				}
			}
			for (const auto ent : uncategorized) {
				auto resolutions = *ent->getRequestedResolutions(ent->_remoteAddress);
				for (const auto& resReq : resolutions) {
					requestVec->push_back(resReq);
				}
			}
			return requestVec;
		}
	};
}