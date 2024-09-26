#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class Alteria : public DMARender::IGameMap {


	public:
		Alteria() : DMARender::IGameMap(std::string("Alteria"), std::string("maps\\alteria.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}