#include "OverlayAdapter.h"
#include <imgui.h> 

bool DayZ::OverlayAdapter::WorldToScreenDayZ(DayZ::Camera* camera, const DMARender::Vector3& position, DMARender::Vector2& outVector)
{
	if (!camera) return false;


	DMARender::Vector3 temp = DMARender::Vector3(
		position.x - camera->InvertedViewTranslation.x,
		position.y - camera->InvertedViewTranslation.y,
		position.z - camera->InvertedViewTranslation.z
	);

	float x = temp.Dot(camera->InvertedViewRight);
	float y = temp.Dot(camera->InvertedViewUp);
	float z = temp.Dot(camera->InvertedViewForward);

	if (z < 0.65f) return false;


	ImVec2 windowSize = ImGui::GetWindowSize();


	float normalizedX = (x / camera->GetProjectionD1.x) / z;
	float normalizedY = (y / camera->GetProjectionD2.y) / z;


	outVector.x = (windowSize.x / 2) + (normalizedX * (windowSize.x / 2));
	outVector.y = (windowSize.y / 2) - (normalizedY * (windowSize.y / 2));
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
	drawLoot(camera.get(), nearTable->resolvedEntities);
	drawLoot(camera.get(), farTable->resolvedEntities);
	drawAliveEntities(camera.get(), nearTable->resolvedEntities, scoreboard.get());
	drawAliveEntities(camera.get(), farTable->resolvedEntities, scoreboard.get());
	// drawDebugInformation(camera.get(), slowTable->resolvedEntities);
	// drawDebugInformation(camera.get(), itemTable->resolvedEntities);
	// drawDebugInformation(camera.get(), nearTable->resolvedEntities);
	// drawDebugInformation(camera.get(), farTable->resolvedEntities);

}

void DayZ::OverlayAdapter::createFonts()
{
	ImFontConfig config;
	config.OversampleH = 3;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;
	lootFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 10, &config);
	lootFontFar = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 10, &config);
	playerFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 10, &config);
}

void DayZ::OverlayAdapter::drawAliveEntities(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities, DayZ::Scoreboard* scoreboard)
{


	ImGui::PushFont(playerFont);


	std::vector<std::string> specialPlayerNames = { "Bkmmhz", "BuckHammond", "Friend3", "Friend4", "Friend5 " };

	for (const auto& ent : entities) {
		//Draw Bounding Box
		if (!ent->isValid())
			continue;
		if (ent->isDead)
			continue;


		ImU32 boxColor;

		//bool ismainplayPlayer = false;
		bool isSpecialPlayer = false;

		std::vector<std::string> infoText;

		if (ent->isPlayer()) {
			auto ident = ent->getPlayerIdentity(scoreboard);
			if (ident && ident->PlayerName) {
				// If the player's name is in the list of special names, mark them as special
				isSpecialPlayer = std::find(specialPlayerNames.begin(), specialPlayerNames.end(), ident->PlayerName->value) != specialPlayerNames.end();
			}
		}
		if (isSpecialPlayer) {
			boxColor = IM_COL32(0, 255, 0, 255);
		}
		else if (ent->isPlayer()) {
			boxColor = IM_COL32(255, 0, 0, 255);
		}
		//else if (ent->isZombie()) {
			//boxColor = IM_COL32(255, 255, 0, 255);
		//}
		else if (ent->isAnimal()) {
			boxColor = IM_COL32(0, 255, 0, 200);
		}
		else {
			continue;
		}
		auto originPos = ent->FutureVisualStatePtr->position;
		float entHeight = ent->isAnimal() ? 1 : 1.8;
		auto topPos = originPos + DMARender::Vector3(0, entHeight, 0);

		DMARender::Vector2 originW2S, topW2S;
		float dist = camera->InvertedViewTranslation.Dist(ent->FutureVisualStatePtr->position);
		if (dist < 4)
			continue;
		//if (ent->isZombie() && dist > 100)
		//	continue;

		if (!WorldToScreenDayZ(camera, originPos, originW2S))
			continue;
		if (!WorldToScreenDayZ(camera, topPos, topW2S))
			continue;
		float width = (originW2S.y - topW2S.y) / entHeight;
		drawBoundingBox(topW2S, originW2S, width, boxColor);

		//std::vector<std::string> infoText;
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

		drawTextList(infoText, DMARender::Vector2(topW2S.x + width, topW2S.y), ImGui::GetFontSize(), boxColor);
	}
	ImGui::PopFont();
}

void DayZ::OverlayAdapter::drawDebugInformation(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities)
{
	for (auto const item : entities) {
		auto itemPos = item->FutureVisualStatePtr->position;
		auto screenPos = DMARender::Vector2();
		if (!WorldToScreenDayZ(camera, itemPos, screenPos))
			continue;
		drawText(std::string("TypeName: ") + item->EntityTypePtr->TypeName->value, DMARender::Vector2(screenPos.x, screenPos.y + 40), ImGui::GetFontSize(), IM_COL32(0, 255, 0, 255));
		drawText(std::string("ConfigName: ") + item->EntityTypePtr->ConfigName->value, DMARender::Vector2(screenPos.x, screenPos.y + 60), ImGui::GetFontSize(), IM_COL32(0, 255, 0, 255));;
		drawText(std::string("CleanName: ") + item->EntityTypePtr->CleanName->value, DMARender::Vector2(screenPos.x, screenPos.y + 80), ImGui::GetFontSize(), IM_COL32(0, 255, 0, 255));;
		drawText(std::string("ModelName: ") + item->EntityTypePtr->ModelName->value, DMARender::Vector2(screenPos.x, screenPos.y + 100), ImGui::GetFontSize(), IM_COL32(0, 255, 0, 255));;
		drawText(std::to_string(item->isDead), DMARender::Vector2(screenPos.x, screenPos.y + 120), ImGui::GetFontSize(), IM_COL32(0, 255, 0, 255));;
	}
}

void DayZ::OverlayAdapter::drawLoot(DayZ::Camera* camera, const std::vector<std::shared_ptr<DayZ::Entity>>& entities) {


	for (auto const item : entities) {
		if (!item->isValid())
			continue;
		if (
			!(item->isGroundItem()) &&
			!(item->isPlayer() && item->isDead) &&
			//!(item->isZombie() && item->isDead) &&
			!(item->isAnimal() && item->isDead)
			) {
			continue;
		}
		float maxDist = 10000;
		std::string postFix = "";
		ImU32 textCol;
		if (item->isPlayer()) {
			textCol = IM_COL32(0, 255, 255, 150);
			maxDist = 2000;
			postFix = " (Dead)";
		}
		else if (item->isAnimal()) {
			textCol = IM_COL32(0, 255, 0, 255);
			maxDist = 1000;
			postFix = " (Dead)";
		}
		//else if (item->isZombie()) {
		//	textCol = IM_COL32(255, 255, 0, 255);
		//	maxDist = 50;
		//	postFix = " (Dead)";
		//}
		else if (item->isGroundItem()) {
			textCol = IM_COL32(255, 255, 255, 255);
			maxDist = 100;
		}
		else {
			continue;
		}

		auto itemPos = item->FutureVisualStatePtr->position;
		auto screenPos = DMARender::Vector2();
		if (!WorldToScreenDayZ(camera, itemPos, screenPos))
			continue;
		float dist = camera->InvertedViewTranslation.Dist(item->FutureVisualStatePtr->position);
		if (dist > maxDist)
			continue;
		if (dist > 50) {
			ImGui::PushFont(lootFontFar);
		}
		else {
			ImGui::PushFont(lootFont);
		}


		drawText(item->EntityTypePtr->getBestString()->value + postFix, screenPos, ImGui::GetFontSize(), textCol);
		ImGui::PopFont();

	}
}