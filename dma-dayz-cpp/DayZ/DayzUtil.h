#pragma once
#include "../DMAMemoryManagement/MemProcFS/vmmdll.h";
namespace DayZ {
	class Util {
	public:
		static bool isPointerValid(QWORD ptr) {
			if (ptr >= 0x200000009 && ptr < 0xffffffff00000000)
				return true;
			return false;
		}
	};
}