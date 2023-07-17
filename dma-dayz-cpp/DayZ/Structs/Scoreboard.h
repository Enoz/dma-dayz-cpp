#pragma once
#include "../../DMAMemoryManagement/includes.h";
#include "ScoreboardIdentity.h";
#include <set>;
#include "../DayzUtil.h";

namespace DayZ {
	class Scoreboard : public DMAMem::MemoryObject {
		QWORD ScoreboardIdentityPointers[60];
	public:
		std::vector<std::shared_ptr<ScoreboardIdentity>> resolvedIdentities;
		Scoreboard() {
			this->registerOffset(0x0, &ScoreboardIdentityPointers, sizeof(QWORD[60]));
		}

		std::shared_ptr<std::vector<DMAMem::MemoryObject::ResolutionRequest>> getRequestedResolutions(QWORD baseAddress) override {
			if (!_isBaseResolved) {
				return generateDefaultResolutions(baseAddress);
			}

			if (resolvedIdentities.size() > 0) {
				std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
				for (auto const ident : resolvedIdentities) {
					DMAUtils::concatVectors(requestVec.get(), ident->getRequestedResolutions(ident->_lastAddressUsed).get());
				}
				return requestVec;
			}
			std::shared_ptr< std::vector<ResolutionRequest>> requestVec(new std::vector<ResolutionRequest>());
			std::set<QWORD> vecEntityPointers(std::begin(ScoreboardIdentityPointers), std::end(ScoreboardIdentityPointers));
			for (QWORD identityPtr : vecEntityPointers) {
				if (DayZUtil::isPointerValid(identityPtr)) {
					auto ent = std::shared_ptr<ScoreboardIdentity>(new ScoreboardIdentity());
					DMAUtils::concatVectors(requestVec.get(), ent->getRequestedResolutions(identityPtr).get());
					resolvedIdentities.push_back(ent);
				}
			}
			return requestVec;
		}
	};
}