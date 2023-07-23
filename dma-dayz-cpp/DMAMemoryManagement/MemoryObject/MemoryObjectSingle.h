#pragma once
#include "./MemoryObject.h"

namespace DMAMem {
	template <typename T>
	class MemoryObjectSingle : public MemoryObject {
	public:
		T obj;
		MemoryObjectSingle() {
			this->registerOffset(0x0, &obj, sizeof(T));
		}
	};
}