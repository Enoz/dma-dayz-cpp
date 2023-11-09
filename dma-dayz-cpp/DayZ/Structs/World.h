#pragma once
#include "DMAMemoryManagement/includes.h";
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

	protected:
		std::vector<DMAMem::MemoryObject::ResolutionRequest> postResolveResolutions() override {
			if (NearEntityTable == nullptr) {
				NearEntityTable = std::shared_ptr<EntityTable>(new EntityTable(this->NearEntityTableCount));
				FarEntityTable = std::shared_ptr<EntityTable>(new EntityTable(this->FarEntityTableCount));
				SlowEntityTable = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(this->SlowEntityCountAlloc, this->SlowEntityValidCount));
				ItemTable = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(this->ItemTableCountAlloc, this->ItemTableCount));
				

				this->registerPointer(0xF48, NearEntityTable.get());
				this->registerPointer(0x1090, FarEntityTable.get());
				this->registerPointer(0x2010, SlowEntityTable.get());
				this->registerPointer(0x2060, ItemTable.get());
				return this->getRequestedResolutions(_remoteAddress);
			}
			return this->getPointerResolutions();
		}

	public:

		World() {
			this->registerOffset(0xF48 + 0x8, &NearEntityTableCount, sizeof(int32_t));
			this->registerOffset(0x1090 + 0x8, &FarEntityTableCount, sizeof(int32_t));
			this->registerOffset(0x2010 + 0x8, &SlowEntityCountAlloc, sizeof(int32_t));
			this->registerOffset(0x1F90 + 0x8 + 0x8, &SlowEntityValidCount, sizeof(int32_t));
			this->registerOffset(0x2060 + 0x8, &ItemTableCountAlloc, sizeof(int32_t));
			this->registerOffset(0x2060 + 0x8 + 0x8, &ItemTableCount, sizeof(int32_t));

			camera = std::shared_ptr<Camera>(new Camera());
			this->registerPointer(0x1B8, camera.get());
		}
		std::shared_ptr<EntityTable> NearEntityTable = nullptr;
		std::shared_ptr<Camera> camera = nullptr;
		std::shared_ptr<EntityTable> FarEntityTable = nullptr;
		std::shared_ptr<EntityTableSlowItem> SlowEntityTable = nullptr;
		std::shared_ptr<EntityTableSlowItem> ItemTable = nullptr;
		int32_t NearEntityTableCount;
		int32_t FarEntityTableCount;
		int32_t SlowEntityCountAlloc;
		int32_t SlowEntityValidCount;
		int32_t ItemTableCountAlloc;
		int32_t ItemTableCount;

	};

}