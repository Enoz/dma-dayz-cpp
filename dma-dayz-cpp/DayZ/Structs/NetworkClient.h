#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "Scoreboard.h";


namespace DayZ {
	class NetworkClient : public DMAMem::MemoryObject {

	public:

		uint16_t PlayerCount;
		std::shared_ptr<Scoreboard> scoreboardPtr;

		NetworkClient() {
			this->registerOffset(0x18, &PlayerCount, sizeof(uint16_t));

			scoreboardPtr = std::shared_ptr<Scoreboard>(new Scoreboard());
			this->registerPointer(0x10, scoreboardPtr.get());
		}

		NetworkClient(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : NetworkClient() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
		}
	};
}