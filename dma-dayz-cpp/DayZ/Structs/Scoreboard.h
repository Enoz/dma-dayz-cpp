#pragma once
#include "DMAMemoryManagement/includes.h";
#include "ScoreboardIdentity.h";
#include <set>;
#include "../DayzUtil.h";

namespace DayZ {
	class Scoreboard : public DMAMem::MemoryObject {
		QWORD ScoreboardIdentityPointers[120];
	protected:
		std::vector<DMAMem::MemoryObject::ResolutionRequest> postResolveResolutions() override {
			if (resolvedIdentities.size() > 0) {
				std::vector<ResolutionRequest> requestVec;
				for (auto const ident : resolvedIdentities) {
					auto entRes = ident->getRequestedResolutions(ident->_remoteAddress);
					DMAUtils::concatVectors(&requestVec, &entRes);
				}
				return requestVec;
			}
			std::vector<ResolutionRequest> requestVec;
			std::set<QWORD> vecEntityPointers(std::begin(ScoreboardIdentityPointers), std::end(ScoreboardIdentityPointers));
			for (QWORD identityPtr : vecEntityPointers) {
				if (DayZUtil::isPointerValid(identityPtr)) {
					auto ent = std::shared_ptr<ScoreboardIdentity>(new ScoreboardIdentity());
					auto entRes = ent->getRequestedResolutions(identityPtr);
					DMAUtils::concatVectors(&requestVec, &entRes);
					resolvedIdentities.push_back(ent);
				}
			}
			return requestVec;
		}

	public:
		std::vector<std::shared_ptr<ScoreboardIdentity>> resolvedIdentities;
		Scoreboard() {
			this->registerOffset(0x0, &ScoreboardIdentityPointers, sizeof(QWORD[120]));
		}

	};
}