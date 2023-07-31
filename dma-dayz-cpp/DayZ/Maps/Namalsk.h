#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class Namalsk : public DMARender::IGameMap {


	public:
		Namalsk() : DMARender::IGameMap(std::string("Namalsk"), std::string("maps\\namalsk.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}