#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class Lux : public DMARender::IGameMap {


	public:
		Lux() : DMARender::IGameMap(std::string("Lux"), std::string("maps\\lux.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}