#pragma once
#include <map>
#include "../MemProcFS/vmmdll.h"
#include "../VmmManager/VmmManager.h"
#include <vector>
#include <memory>

#define GAME_POINTER_TYPE DWORD
#define GAME_POINTER_SIZE sizeof(GAME_POINTER_TYPE)

namespace DMAMem {
	class MemoryObject {
	protected:
		void registerOffset(int offset, void* destination, int typeSize);
		void registerPointer(int offset, MemoryObject* destination);
	private:
		struct OffsetEntry {
			int offset;
			void* destination;
			int typeSize;
		};
		struct OffsetPointer {
			int offset;
			MemoryObject* destination;
		};
		std::vector<OffsetEntry> offsetVector;
		std::vector<OffsetPointer> pointerVector;
	public:
		int getObjectSize();
		BOOL resolveOffsets(VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress);
		BOOL resolveObject(VmmManager* vmmManager, DWORD remotePid, char* objectData);
	};
}
