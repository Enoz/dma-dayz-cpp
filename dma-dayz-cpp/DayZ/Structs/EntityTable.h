#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Entity.h";

namespace DayZ {
	class EntityTable : public DMAMem::MemoryObjectScatterable {
		QWORD EntityPointers[60];
	public:
		std::vector<std::shared_ptr<Entity>> resolvedIdentities;
		EntityTable() {
			this->registerOffset(0x0, &EntityPointers, sizeof(QWORD[60]));
		}

		EntityTable(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : EntityTable() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
		}

		void postPointerResolution(DMAMem::VmmManager* vmmManager, DWORD remotePid) {
			resolvedIdentities = std::vector<std::shared_ptr<Entity>>();
			this->initializeScatter();
			for (QWORD entityPtr : EntityPointers) {
				if (entityPtr != NULL) {
					auto ent = std::shared_ptr<Entity>(new Entity());
					this->registerScatterObject(ent.get(), entityPtr);
					resolvedIdentities.push_back(ent);
				}
			}
			this->populateScatterObjects(vmmManager, remotePid);
		}
	};
}