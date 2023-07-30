#include "RadarAdapter.h"

DayZ::RadarAdapter::RadarAdapter(DayZ::MemoryUpdater* memUpdater)
{
	this->memUpdater = memUpdater;
}

void DayZ::RadarAdapter::DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform)
{
	auto drawList = ImGui::GetBackgroundDrawList();
	auto nearEnts = this->memUpdater->getNearEntityTable()->resolvedEntities;
	ImGui::Begin("Debug Radar");
	for (auto ent : nearEnts) {
		ImVec2 screenPos;
		if (WorldToRadar(curMap, mTransform, ent->FutureVisualStatePtr->position, screenPos)) {
			drawList->AddCircleFilled(screenPos, 10, IM_COL32(255, 0, 0, 255), 10);
			ImGui::Text("GamePos(%.1f, %.1f, %.1f), ScreenPos: (%.1f, %.1f)", ent->FutureVisualStatePtr->position.x, ent->FutureVisualStatePtr->position.y, ent->FutureVisualStatePtr->position.z, screenPos.x, screenPos.y);
		}
	}
	ImGui::End();
}
