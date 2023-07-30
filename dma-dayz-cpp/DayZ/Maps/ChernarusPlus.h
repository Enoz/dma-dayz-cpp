#pragma once
#include "DMARender/includes.h"
namespace DayZ {
	class ChernarusPlus : public DMARender::IGameMap {
		

	public:
		ChernarusPlus() : DMARender::IGameMap(std::string("Chernarus Plus"), std::string("D:\\reverse-projects\\DayZ\\maps\\chernarusplus\\layers\\map.png")) {}

		// Inherited via IGameMap
		virtual ImVec2 translatePoint(const DMARender::Vector3& gamePoint) override;
	};
}