#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class Esseker : public DMARender::IGameMap {


	public:
		Esseker() : DMARender::IGameMap(std::string("Esseker"), std::string("maps\\esseker.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}