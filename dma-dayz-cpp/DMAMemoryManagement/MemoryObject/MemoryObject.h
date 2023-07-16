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


		struct ScatterEntry {
			MemoryObject* memObj;
			QWORD remoteAddress;
		};
		struct ResolvedScatterEntry {
			ScatterEntry se;
			std::shared_ptr<char[]> objectData;
		};
		std::vector<ScatterEntry> scatterEntries;



	public:
		QWORD _remoteAddress = NULL;
		int getObjectSize();
		BOOL resolveOffsets(VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress, ULONG64 flags = VMMDLL_FLAG_NOCACHE);
		BOOL resolveObject(VmmManager* vmmManager, DWORD remotePid, char* objectData);
		void initializeScatter();
		void registerScatterObject(MemoryObject* memObj, QWORD remoteAddress);
		void populateScatterObjects(VmmManager* vmmManager, DWORD remotePid, ULONG64 flags = VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);
		virtual void postPointerResolution(VmmManager* vmmManager, DWORD remotePid) {};
	};
}
