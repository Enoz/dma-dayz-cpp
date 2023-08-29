#pragma once
#include "DMAMemoryManagement/includes.h";
#include "Scoreboard.h";


namespace DayZ {
	struct NetworkClient : public DMAMem::MemoryObject {

		uint32_t PlayerCount;
		std::shared_ptr<Scoreboard> scoreboardPtr;

		NetworkClient() {
			this->registerOffset(0x20, &PlayerCount, sizeof(uint32_t));
			scoreboardPtr = std::shared_ptr<Scoreboard>(new Scoreboard());
			this->registerPointer(0x18, scoreboardPtr.get());
		}
	};
}