#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "EntityTable.h";
#include "EntityTableSlowItem.h"
#include "Camera.h";
//
//class World
//{
//public:
//	char pad_0000[440]; //0x0000
//	class Camera* CameraPtr; //0x01B8
//	char pad_01C0[3320]; //0x01C0
//	class EntityTable* NearEntityTablePtr; //0x0EB8
//	int32_t NearEntityTableCount; //0x0EC0
//	int32_t N00000FED; //0x0EC4
//	char pad_0EC8[312]; //0x0EC8
//	class EntityTable* FarEntityTable; //0x1000
//	int32_t FarEntityTableCount; //0x1008
//	char pad_100C[3956]; //0x100C
//	class EntityTable* SlowEntityTable; //0x1F80
//	int32_t SlowEntityCountMax; //0x1F88
//	int32_t SlowEntityCount; //0x1F8C
//	char pad_1F90[64]; //0x1F90
//	class ItemList* ItemTable; //0x1FD0
//	int32_t ItemTableCountMax; //0x1FD8
//	int32_t ItemTableCount; //0x1FDC
//	char pad_1FE0[104]; //0x1FE0
//}; //Size: 0x2048

namespace DayZ {
	class World : public DMAMem::MemoryObject {

		bool resolvedBase = false;
		bool resolvedTables = false;
		std::vector<DMAMem::MemoryObject::ResolutionRequest> getRequestedResolutions(QWORD baseAddress) override {
			if (!resolvedBase) {
				this->registerOffset(0xEC0, &NearEntityTableCount, sizeof(int32_t));
				this->registerOffset(0x1008, &FarEntityTableCount, sizeof(int32_t));
				this->registerOffset(0x1F88, &SlowEntityCountAlloc, sizeof(int32_t));
				this->registerOffset(0x1F90, &SlowEntityValidCount, sizeof(int32_t));
				this->registerOffset(0x1FD8, &ItemTableCountAlloc, sizeof(int32_t));
				this->registerOffset(0x1FE0, &ItemTableCount, sizeof(int32_t));



				resolvedBase = true;
				this->_isBaseResolved = false;
				return this->generateDefaultResolutions(baseAddress);

			}
			if (!resolvedTables) {
				NearEntityTable = std::shared_ptr<EntityTable>(new EntityTable(this->NearEntityTableCount));
				FarEntityTable = std::shared_ptr<EntityTable>(new EntityTable(this->FarEntityTableCount));
				SlowEntityTable = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(this->SlowEntityCountAlloc, this->SlowEntityValidCount));
				ItemTable = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(this->ItemTableCountAlloc, this->ItemTableCount));
				camera = std::shared_ptr<Camera>(new Camera());

				this->registerPointer(0xEB8, NearEntityTable.get());
				this->registerPointer(0x1000, FarEntityTable.get());
				this->registerPointer(0x1F80, SlowEntityTable.get());
				this->registerPointer(0x1FD0, ItemTable.get());
				this->registerPointer(0x1B8, camera.get());

				resolvedTables = true;
				this->_isBaseResolved = false;
				return this->generateDefaultResolutions(baseAddress);
				
			}
			auto res = this->generateDefaultResolutions(baseAddress);
			this->_isBaseResolved = true;
			return res;
			
		}

	public:
		std::shared_ptr<EntityTable> NearEntityTable;
		int32_t NearEntityTableCount;
		std::shared_ptr<EntityTable> FarEntityTable;
		int32_t FarEntityTableCount;
		std::shared_ptr<EntityTableSlowItem> SlowEntityTable;
		int32_t SlowEntityCountAlloc;
		int32_t SlowEntityValidCount;
		std::shared_ptr<EntityTableSlowItem> ItemTable;
		int32_t ItemTableCountAlloc;
		int32_t ItemTableCount;
		std::shared_ptr<Camera> camera;

		World() {

		}
	};

}