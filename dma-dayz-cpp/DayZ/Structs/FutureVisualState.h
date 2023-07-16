#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Vectors.h";

namespace DayZ {
	struct FutureVisualState : public DMAMem::MemoryObject {
		Vector3 position;

		FutureVisualState() {
			this->registerOffset(0x2c, &position, sizeof(Vector3));
		}

		FutureVisualState(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : FutureVisualState() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
		}
	};
}