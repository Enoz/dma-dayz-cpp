#include "RadarAdapter.h"

DayZ::RadarAdapter::RadarAdapter(DayZ::MemoryUpdater* memUpdater)
{
	this->memUpdater = memUpdater;
}

void DayZ::RadarAdapter::DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform)
{
	auto drawList = ImGui::GetBackgroundDrawList();
	auto nearEnts = this->memUpdater->getNearEntityTable()->resolvedEntities;
	for (auto ent : nearEnts) {
		ImVec2 screenPos = WorldToRadar(curMap, mTransform, ent->FutureVisualStatePtr->position);
		//drawList->AddCircleFilled(screenPos, 10, IM_COL32(255, 0, 0, 255), 10);
		drawBlip(screenPos, 6, IM_COL32(255, 0, 0, 255));
	}
}
