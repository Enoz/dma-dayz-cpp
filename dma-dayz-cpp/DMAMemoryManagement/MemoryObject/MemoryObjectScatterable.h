#pragma once
#include "MemoryObject.h";

namespace DMAMem {
	class MemoryObjectScatterable : public MemoryObject {
	private:
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
		void initializeScatter();
		void registerScatterObject(MemoryObject* memObj, QWORD remoteAddress);
		void populateScatterObjects(VmmManager* vmmManager, DWORD remotePid);

	};
}