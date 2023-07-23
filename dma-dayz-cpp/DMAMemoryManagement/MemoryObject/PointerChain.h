#pragma once
#include "./MemoryObject.h"


namespace DMAMem {
	//TODO: constraint this to of base class MemoryObject
	template <typename T>
	class PointerChain : public MemoryObject {
	private:
		std::vector<int> offsetArray;
		std::vector<std::shared_ptr<QWORD>> resolvedPointerArray;
;	public:
		std::shared_ptr<T> ptr;
		PointerChain(std::vector<int> offsetArray) {
			this->offsetArray = offsetArray;
			ptr = std::shared_ptr<T>(new T());
			this->registerPointer(NULL, (MemoryObject*)ptr.get());
		}

		std::vector<DMAMem::MemoryObject::ResolutionRequest> getRequestedResolutions(QWORD baseAddress) override {

			if (offsetArray.size() == resolvedPointerArray.size()) {
				MemoryObject* finalObj = ((MemoryObject*)ptr.get());
				if (resolvedPointerArray.size() == 0) {
					return finalObj->getRequestedResolutions(baseAddress);
				}
				return finalObj->getRequestedResolutions(*resolvedPointerArray.back());
			}

			int curOffset = offsetArray[resolvedPointerArray.size()];
			ResolutionRequest resReq;
			
			if (resolvedPointerArray.size() == 0) {
				resReq.address = baseAddress + curOffset;
			}
			else {
				resReq.address = *resolvedPointerArray.back() + curOffset;
			}

			//Destination
			std::shared_ptr<QWORD> destination(new QWORD());
			resReq.destination = destination.get();
			resolvedPointerArray.push_back(destination);

			resReq.size = sizeof(QWORD);
			std::vector<ResolutionRequest> retVec {resReq};
			return retVec;
		}

	};
}