#pragma once
#include <map>
#include "../MemProcFS/vmmdll.h"
#include "../VmmManager/VmmManager.h"
#include <vector>
#include <memory>

#define GAME_POINTER_TYPE QWORD
#define GAME_POINTER_SIZE sizeof(GAME_POINTER_TYPE)

namespace DMAMem {
	class MemoryObject {
	protected:
		void registerOffset(int offset, void* destination, int typeSize);
		void registerPointer(int offset, MemoryObject* destination, ULONG64 flags = VMMDLL_FLAG_NOCACHE);

	private:
		struct OffsetEntry {
			int offset;
			void* destination;
			int typeSize;
		};
		struct OffsetPointer {
			int offset;
			MemoryObject* destination;
			ULONG64 flags;
		};
		std::vector<OffsetEntry> offsetVector;
		std::vector<OffsetPointer> pointerVector;

	public:
		QWORD lastRemoteAddressUsed = NULL;
		int getObjectSize();
		BOOL resolveOffsets(VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress, ULONG64 flags = VMMDLL_FLAG_NOCACHE);
		BOOL resolveObject(VmmManager* vmmManager, DWORD remotePid, char* objectData);
		virtual void postPointerResolution(VmmManager* vmmManager, DWORD remotePid) {};
		void updateResolution(VmmManager* vmmManager, DWORD remotePid);
	};
}
