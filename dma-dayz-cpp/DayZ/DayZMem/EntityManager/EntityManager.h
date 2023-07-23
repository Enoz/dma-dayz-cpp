#pragma once
#include "../../../DMAMemoryManagement/includes.h"
#include "../../Structs/EntityTable.h"
#include "../../Structs/EntityTableSlowItem.h"
#include "../../Structs/WorldNoLists.h"

namespace DayZ {
	class EntityManager : public DMAMem::MemoryObject {
		QWORD worldAddress;
		DMAMem::VmmManager* manager;
		DWORD pid;
		std::shared_ptr<EntityTable> NearEntityTable;
		std::shared_ptr<EntityTable> FarEntityTable;
		std::shared_ptr<EntityTableSlowItem> SlowEntityTable;
		std::shared_ptr<EntityTableSlowItem> ItemTable;
	public:
		EntityManager(QWORD worldAddress, DMAMem::VmmManager* manager, DWORD pid) {
			this->worldAddress = worldAddress;
			this->manager = manager;
			this->pid = pid;
		}

		void refreshAll() {
			refreshNear();
			refreshFar();
			refreshItem();
			refreshSlow();
		}

		DayZ::WorldNoLists getWNL() {
			WorldNoLists wnl = WorldNoLists();
			wnl.resolveObject(manager, pid, worldAddress);
			return wnl;
		}

		void refreshNear() {
			auto wnl = getWNL();
			NearEntityTable = std::shared_ptr<EntityTable>(new EntityTable(wnl.NearEntityTableCount));
			NearEntityTable->resolveObject(manager, pid, wnl.NearTableAddress);
		}
		void refreshFar() {
			auto wnl = getWNL();
			FarEntityTable = std::shared_ptr<EntityTable>(new EntityTable(wnl.FarEntityTableCount));
			NearEntityTable->resolveObject(manager, pid, wnl.FarTableAddress);
		}
		void refreshSlow() {
			auto wnl = getWNL();
			SlowEntityTable = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(wnl.SlowEntityCountAlloc, wnl.SlowEntityValidCount));
			NearEntityTable->resolveObject(manager, pid, wnl.SlowTableAddress);
		}
		void refreshItem() {
			auto wnl = getWNL();
			ItemTable = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(wnl.ItemTableCountAlloc, wnl.ItemTableCount));
			NearEntityTable->resolveObject(manager, pid, wnl.ItemTableAddress);
		}


	};
}