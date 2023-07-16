#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "ScoreboardIdentity.h";

namespace DayZ {
	class Scoreboard : public DMAMem::MemoryObjectScatterable {
		QWORD ScoreboardIdentityPointers[60];
	public:
		std::vector<std::shared_ptr<ScoreboardIdentity>> resolvedIdentities;
		Scoreboard() {
			this->registerOffset(0x0, &ScoreboardIdentityPointers, sizeof(QWORD[60]));
		}

		Scoreboard(DMAMem::VmmManager* vmmManager, DWORD remotePid, QWORD remoteAddress) : Scoreboard() {
			this->resolveOffsets(vmmManager, remotePid, remoteAddress);

			resolvedIdentities = std::vector<std::shared_ptr<ScoreboardIdentity>>();
			this->initializeScatter();
			for (QWORD identityPointer : ScoreboardIdentityPointers) {
				if (identityPointer != NULL) {
					auto ident = std::shared_ptr<ScoreboardIdentity>(new ScoreboardIdentity());
					this->registerScatterObject(ident.get(), identityPointer);
					resolvedIdentities.push_back(ident);
				}
			}
			this->populateScatterObjects(vmmManager, remotePid);
		}
	};
}