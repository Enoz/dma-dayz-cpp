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
	auto slowTable = memUpdater->getSlowEntityTable();
	auto nearTable = memUpdater->getNearEntityTable();
	auto farTable = memUpdater->getFarEntityTable();
	auto scoreboard = memUpdater->getScoreboard();
	drawLoot(camera.get(), itemTable->resolvedEntities);
	drawLoot(camera.get(), slowTable->resolvedEntities);
	drawAliveEntities(camera.get(), nearTable->resolvedEntities, scoreboard.get());
	drawAliveEntities(camera.get(), farTable->resolvedEntities, scoreboard.get());

}

void DayZ::OverlayAdapter::createFonts()
{
	ImFontConfig config;
	config.OversampleH = 3;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;
	lootFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 14, &config);
	lootFontFar = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 12, &config);
	playerFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 12, &config);
}

void DayZ::OverlayAdapter::drawAliveEntities(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities, DayZ::Scoreboard* scoreboard)
{
	ImGui::PushFont(playerFont);
	for (const auto& ent : entities) {
		//Draw Bounding Box
		ImU32 boxColor;
		if (ent->isPlayer()) {
			boxColor = IM_COL32(255, 0, 0, 255);
		}
		if (ent->isZombie()) {
			boxColor = IM_COL32(255, 255, 0, 255);
		}
		if (ent->isAnimal()) {
			boxColor = IM_COL32(0, 255, 0, 255);
		}
		auto originPos = ent->FutureVisualStatePtr->position;
		float entHeight = ent->isAnimal() ? 1 : 1.8;
		auto topPos = originPos + DMARender::Vector3(0, entHeight, 0);

		DMARender::Vector2 originW2S, topW2S;
		float dist = camera->InvertedViewTranslation.Dist(ent->FutureVisualStatePtr->position);
		if (dist < 3.5)
			continue;
		if (ent->isZombie() && dist > 100)
			continue;

		if (!WorldToScreen(camera, originPos, originW2S))
			continue;
		if (!WorldToScreen(camera, topPos, topW2S))
			continue;
		float width = (originW2S.y - topW2S.y) / entHeight;
		DMARender::Utils::drawBoundingBox(topW2S, originW2S, width, boxColor);

		std::vector<std::string> infoText;
		if (ent->isAnimal()) {
			auto entBestStr = ent->EntityTypePtr->getBestString();
			if (entBestStr) {
				auto entName = std::string(ent->EntityTypePtr->getBestString()->value);
				infoText.push_back(entName);
			}

		}
		if (ent->isPlayer()) {
			auto ident = ent->getPlayerIdentity(scoreboard);
			if (ident && ident->PlayerName) {
				infoText.push_back(ident->PlayerName->value);
			}

			if (ent->InventoryPtr->isHandItemValid) {
				auto bestHandStr = ent->InventoryPtr->handItem->EntityTypePtr->getBestString();
				if (bestHandStr) {
					infoText.push_back(bestHandStr->value);
				}
				else {
					infoText.push_back("(Unknown)");
				}
			}
			
			infoText.push_back(std::format("{:.1f}", dist));
		}




		DMARender::Utils::drawTextList(infoText, DMARender::Vector2(topW2S.x + width, topW2S.y), ImGui::GetFontSize(), boxColor);
	}
	ImGui::PopFont();
}

void DayZ::OverlayAdapter::drawLoot(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities) {


	for (auto const item : entities) {
		if (!item->isValid())
			continue;
		if (
			!(item->isGroundItem()) &&
			!(item->isPlayer() && item->isDead) &&
			!(item->isZombie() && item->isDead) &&
			!(item->isAnimal() && item->isDead)
			) {
			continue;
		}
		float maxDist = 10000;
		std::string postFix = "";
		ImU32 textCol;
		if (item->isPlayer()) {
			textCol = IM_COL32(255, 0, 0, 255);
			maxDist = 2000;
			postFix = " (Dead)";
		}
		if (item->isAnimal()) {
			textCol = IM_COL32(0, 255, 0, 255);
			maxDist = 2000;
			postFix = " (Dead)";
		}
		if (item->isZombie()) {
			textCol = IM_COL32(255, 255, 0, 255);
			maxDist = 50;
			postFix = " (Dead)";
		}
		if (item->isGroundItem()) {
			textCol = IM_COL32(255, 255, 255, 255);
			maxDist = 2000;
		}

		auto itemPos = item->FutureVisualStatePtr->position;
		auto screenPos = DMARender::Vector2();
		if (!WorldToScreen(camera, itemPos, screenPos))
			continue;
		float dist = camera->InvertedViewTranslation.Dist(item->FutureVisualStatePtr->position);
		if (dist > maxDist)
			continue;
		if (dist > 50) {
			ImGui::PushFont(lootFontFar);
		} else {
			ImGui::PushFont(lootFont);
		}

	
		//float size = 100 / dist;
		//if (size < 12) {
		//	size = 12;
		//}
		try {
			DMARender::Utils::drawText(item->EntityTypePtr->getBestString()->value + postFix, screenPos, ImGui::GetFontSize(), textCol);
		}
		catch (...) {

		}
		ImGui::PopFont();
		
	}
}