#include "RadarAdapter.h"

void DayZ::RadarAdapter::drawLoot(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform, const std::vector<std::shared_ptr<DayZ::Entity>>& entities)
{
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
		std::string postFix = "";
		ImU32 textCol;
		if (item->isPlayer()) {
			textCol = IM_COL32(0, 255, 255, 255);
			postFix = " (Dead)";
		} else if (item->isAnimal()) {
			textCol = IM_COL32(0, 255, 0, 255);
			postFix = " (Dead)";
		} else if (item->isGroundItem()) {
			textCol = IM_COL32(255, 255, 255, 255);
		}
		else {
			continue;
		}

		auto screenPos = WorldToRadar(curMap, mTransform, item->FutureVisualStatePtr->position);

		auto bestName = item->EntityTypePtr->getBestString();
		if (bestName) {
			drawBlip(screenPos, 5, textCol, 14, 1, { bestName->value + postFix });
		}

	}
}

void DayZ::RadarAdapter::drawAliveEntities(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform, const std::vector<std::shared_ptr<DayZ::Entity>>& entities, Scoreboard* scoreboard)
{
	for (auto ent : entities) {
		if (!ent->isValid())
			continue;
		ImU32 blipColor;
		float blipSize;
		if (ent->isPlayer()) {
			blipColor = IM_COL32(255, 0, 0, 255);
			blipSize = 8;
		} else if (ent->isZombie()) {
			blipColor = IM_COL32(255, 255, 0, 255);
			blipSize = 4;
		} else if (ent->isAnimal()) {
			blipColor = IM_COL32(0, 255, 0, 255);
			blipSize = 6;
		}
		else {
			continue;
		}

		std::vector<std::string> infoText;
		ImVec2 screenPos = WorldToRadar(curMap, mTransform, ent->FutureVisualStatePtr->position);
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
		}

		if (ent->isPlayer()) {
			drawBlipDirectional(screenPos, blipSize, blipColor, 16, 1, infoText, ent->FutureVisualStatePtr->getRotationCorrected());
		}
		else {
			drawBlip(screenPos, blipSize, blipColor, 16, 1, infoText);

		}
	}

}

DayZ::RadarAdapter::RadarAdapter(DayZ::MemoryUpdater* memUpdater)
{
	this->memUpdater = memUpdater;
}

void DayZ::RadarAdapter::DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform)
{

	ImGui::PushFont(radarFont);
	drawLoot(curMap, mTransform, memUpdater->getSlowEntityTable()->resolvedEntities);
	drawLoot(curMap, mTransform, memUpdater->getItemTable()->resolvedEntities);
	drawAliveEntities(curMap, mTransform, memUpdater->getFarEntityTable()->resolvedEntities, memUpdater->getScoreboard().get());
	drawAliveEntities(curMap, mTransform, memUpdater->getNearEntityTable()->resolvedEntities, memUpdater->getScoreboard().get());
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
