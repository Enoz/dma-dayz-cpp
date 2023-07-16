#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "NetworkClient.h";
//class NetworkManager
//{
//public:
//	char pad_0000[72]; //0x0000
//	class NetworkClient *N0000016A; //0x0048
//	char pad_0050[2024]; //0x0050
//}; //Size: 0x0838

namespace DayZ {
	class NetworkManager : public DMAMem::MemoryObject {

	public:
		std::shared_ptr<NetworkClient> NetworkClientPtr;

		NetworkManager() {

			NetworkClientPtr = std::shared_ptr<NetworkClient>(new NetworkClient());
			this->registerPointer(0x48, NetworkClientPtr.get());
		}

		NetworkManager(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : NetworkManager() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);
		}
	};
}