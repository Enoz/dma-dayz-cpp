#include "OverlayAdapter.h"

bool DayZ::OverlayAdapter::WorldToScreen(DayZ::Camera* camera, const DMARender::Vector3& position, DMARender::Vector2& outVector)
{
	//convert world cords to screen cords :)

	if (!camera)
		return false;

	DMARender::Vector3 temp = DMARender::Vector3(position.x - camera->InvertedViewTranslation.x,
		position.y - camera->InvertedViewTranslation.y,
		position.z - camera->InvertedViewTranslation.z
	);

	float x = temp.Dot(camera->InvertedViewRight);
	float y = temp.Dot(camera->InvertedViewUp);
	float z = temp.Dot(camera->InvertedViewForward);

	if (z < 0.65f) {
		return false;
	}

	outVector.x = camera->ViewPortSize.x * (1 + (x / camera->GetProjectionD1.x / z));
	outVector.y = camera->ViewPortSize.y * (1 - (y / camera->GetProjectionD2.y / z));
	return true;
}

DayZ::OverlayAdapter::OverlayAdapter(DayZ::MemoryUpdater* memUpdater)
{
	this->memUpdater = memUpdater;


}

void DayZ::OverlayAdapter::DrawOverlay()
{
	auto camera = memUpdater->getCamera();
	auto itemTable = memUpdater->getItemTable();
	drawLoot(camera.get(), itemTable->resolvedEntities);
}

void DayZ::OverlayAdapter::createFonts()
{
	ImFontConfig config;
	config.OversampleH = 2;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;
	lootFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 128, &config);
}

void DayZ::OverlayAdapter::drawLoot(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities) {
	ImGui::PushFont(lootFont);
	for (auto const item : entities) {
		if (!item->isValid())
			continue;
		auto itemPos = item->FutureVisualStatePtr->position;
		auto screenPos = DMARender::Vector2();
		if (!WorldToScreen(camera, itemPos, screenPos))
			continue;
		float dist = camera->InvertedViewTranslation.Dist(item->FutureVisualStatePtr->position);
		float size = 100 / dist;
		if (size < 10) {
			size = 10;
		}
		DMARender::Utils::drawText(item->EntityTypePtr->getBestString()->value, screenPos, size, IM_COL32(255, 255, 255, 255));
	}
	ImGui::PopFont();
}