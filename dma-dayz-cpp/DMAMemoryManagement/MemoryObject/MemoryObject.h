#pragma once
#include <map>
#include "../MemProcFS/vmmdll.h"
#include "../VmmManager/VmmManager.h"
#include "../../DMAMemoryManagement/Utils/Utils.h"
#include <vector>
#include <memory>

#define GAME_POINTER_TYPE QWORD
#define GAME_POINTER_SIZE sizeof(GAME_POINTER_TYPE)

namespace DMAMem {
	class MemoryObject {
	protected:

		struct OffsetEntry {
			int offset;
			void* destination;
			int typeSize;
		};
		struct OffsetPointer {
			int offset;
			MemoryObject* destination;
			GAME_POINTER_TYPE resolvedAddress;
		};
		struct ResolutionRequest {
			QWORD address;
			void* destination;
			int size;
		};


		void registerOffset(int offset, void* destination, int typeSize);
		void registerPointer(int offset, MemoryObject* destination);

		bool _isBaseResolved = false;


		std::shared_ptr<std::vector<DMAMem::MemoryObject::ResolutionRequest>> generateDefaultResolutions(QWORD baseAddress);

	private:
		std::shared_ptr<std::vector<OffsetEntry>> offsetVector = std::shared_ptr<std::vector<OffsetEntry>>(new std::vector<OffsetEntry>());
		std::shared_ptr<std::vector<std::shared_ptr<OffsetPointer>>> pointerVector = std::shared_ptr<std::vector<std::shared_ptr<OffsetPointer>>>(new std::vector<std::shared_ptr<OffsetPointer>>());
		void readResolutions(VmmManager* manager, DWORD pid, std::vector<ResolutionRequest>* resolutionRequests, ULONG64 flags);



	public:
		GAME_POINTER_TYPE _remoteAddress = NULL;
		void resolveObject(VmmManager* manager, DWORD pid, QWORD address, ULONG64 flags = VMMDLL_FLAG_NOCACHE);
		virtual std::shared_ptr<std::vector<DMAMem::MemoryObject::ResolutionRequest>> getRequestedResolutions(QWORD baseAddress);
		void setUnresolved();
	};
}
