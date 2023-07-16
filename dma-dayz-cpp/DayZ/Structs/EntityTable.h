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

		EntityTable(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : EntityTable() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
			this->postPointerResolution(vmmManager, remotePid);
		}

		EntityTable(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress, ULONG64 flags) : EntityTable() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress, flags);
			this->postPointerResolution(vmmManager, remotePid);
		}

		void postPointerResolution(DMAMem::VmmManager* vmmManager, DWORD remotePid) {
			auto resolvedEntitiesTemp = std::vector<std::shared_ptr<Entity>>();
			std::set<QWORD> vecEntityPointers(std::begin(EntityPointers), std::end(EntityPointers));
			this->initializeScatter();
			for (QWORD entityPtr : vecEntityPointers) {
				if (DayZ::Util::isPointerValid(entityPtr)) {
					auto ent = std::shared_ptr<Entity>(new Entity());
					this->registerScatterObject(ent.get(), entityPtr);
					resolvedEntitiesTemp.push_back(ent);
				}
			}
			this->populateScatterObjects(vmmManager, remotePid);

			for (const auto ent : resolvedEntitiesTemp) {
				if (ent->isValid()) {
					resolvedEntities.push_back(ent);
				}
			}
		}
	};
}