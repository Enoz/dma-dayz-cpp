#pragma once
#include "MemoryObject.h"

namespace DMAMem {
	class MemoryObjectPointable : public MemoryObject {
	public:
		virtual void postResolveAction(VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) = 0;
	};
};