#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "EntityTable.h";
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
	struct World : public DMAMem::MemoryObject {
		std::shared_ptr<EntityTable> NearEntityTablePtr;
		int32_t NearEntityTableCount;
		std::shared_ptr<EntityTable> FarEntityTable;
		int32_t FarEntityTableCount;
		std::shared_ptr<EntityTable> SlowEntityTable;
		int32_t SlowEntityCountMax;
		int32_t SlowEntityCount;
		std::shared_ptr<EntityTable> ItemTable;
		int32_t ItemTableCountMax;
		int32_t ItemTableCount;

		World() {
			NearEntityTablePtr = std::shared_ptr<EntityTable>(new EntityTable());
			FarEntityTable = std::shared_ptr<EntityTable>(new EntityTable());
			SlowEntityTable = std::shared_ptr<EntityTable>(new EntityTable());
			ItemTable = std::shared_ptr<EntityTable>(new EntityTable());

			this->registerPointer(0xEB8, NearEntityTablePtr.get());
			this->registerOffset(0xEC0, &NearEntityTableCount, sizeof(int32_t));
			this->registerPointer(0x1000, FarEntityTable.get());
			this->registerOffset(0x1008, &FarEntityTableCount, sizeof(int32_t));
			this->registerPointer(0x1F80, SlowEntityTable.get());
			this->registerOffset(0x1F88, &SlowEntityCountMax, sizeof(int32_t));
			this->registerOffset(0x1F8C, &SlowEntityCount, sizeof(int32_t));
			this->registerPointer(0x1FD0, ItemTable.get());
			this->registerOffset(0x1FD8, &ItemTableCountMax, sizeof(int32_t));
			this->registerOffset(0x1FDC, &ItemTableCount, sizeof(int32_t));

		}

		World(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : World() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
		}
	};
}