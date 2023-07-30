#pragma once
#include "DMARender/includes.h"
#include "../MemoryUpdater/MemoryUpdater.h"
namespace DayZ {
	class OverlayAdapter : public DMARender::IOverlay {

		DayZ::MemoryUpdater* memUpdater;
		bool WorldToScreenDayZ(DayZ::Camera* camera, const DMARender::Vector3& position, DMARender::Vector2& outVector);
		void drawLoot(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities);
		void drawAliveEntities(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities, Scoreboard* scoreboard);
		void drawDebugInformation(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities);
		//Fonts
		ImFont* lootFont;
		ImFont* lootFontFar;
		ImFont* playerFont;
	public:
		OverlayAdapter(DayZ::MemoryUpdater* memUpdater);
		// Inherited via IOverlay
		virtual void DrawOverlay() override;
		virtual void createFonts() override;
	};
}