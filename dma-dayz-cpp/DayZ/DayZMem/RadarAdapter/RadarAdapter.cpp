#include "RadarAdapter.h"

DayZ::RadarAdapter::RadarAdapter(DayZ::MemoryUpdater* memUpdater)
{
	this->memUpdater = memUpdater;
}

void DayZ::RadarAdapter::DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform)
{
	auto drawList = ImGui::GetBackgroundDrawList();
	auto nearEnts = this->memUpdater->getNearEntityTable()->resolvedEntities;
	ImGui::PushFont(radarFont);
	for (auto ent : nearEnts) {
		ImVec2 screenPos = WorldToRadar(curMap, mTransform, ent->FutureVisualStatePtr->position);
		//drawList->AddCircleFilled(screenPos, 10, IM_COL32(255, 0, 0, 255), 10);

		drawBlip(screenPos, 6, IM_COL32(255, 0, 0, 255), 16, 1, {"Test", "One", "Two"});

	}
	ImGui::PopFont();
}

void DayZ::RadarAdapter::createFonts()
{
		ImFontConfig config;
		config.OversampleH = 3;
		config.OversampleV = 1;
		config.GlyphExtraSpacing.x = 1.0f;
		radarFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 32, &config);
}
