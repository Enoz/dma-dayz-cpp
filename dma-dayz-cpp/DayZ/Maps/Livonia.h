#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class Livonia : public DMARender::IGameMap {


	public:
		Livonia() : DMARender::IGameMap(std::string("Livonia"), std::string("maps\\livonia.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}