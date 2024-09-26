#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class DeadFall : public DMARender::IGameMap {


	public:
		DeadFall() : DMARender::IGameMap(std::string("Dead Fall"), std::string("maps\\deadfall.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}