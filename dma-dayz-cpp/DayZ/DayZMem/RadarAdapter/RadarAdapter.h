#pragma once
#include "DMARender/includes.h"
#include "../MemoryUpdater/MemoryUpdater.h"
namespace DayZ {
	class RadarAdapter : public DMARender::IRadar {

		DayZ::MemoryUpdater* memUpdater;
		ImFont* radarFont;
		void drawLoot(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform, const std::vector<std::shared_ptr<DayZ::Entity>>& entities);
		void drawAliveEntities(DayZ::Camera* camera, DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform, const std::vector<std::shared_ptr<DayZ::Entity>>& entities, Scoreboard* scoreboard);
	public:
		RadarAdapter(DayZ::MemoryUpdater* memUpdater);
		// Inherited via IRadar
		virtual void DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform) override;
		virtual void createFonts() override;

	};
}