#include "RadarAdapter.h"
//#include <iostream>
//#include <fstream>
//#include <string>

void DayZ::RadarAdapter::drawLoot(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform, const std::vector<std::shared_ptr<DayZ::Entity>>& entities)
{

	//std::vector<std::string> Listofgunnames = { "AUR A1", "MP5A2", "Desert Eagle", "Colt Python", "AUR AX", "KA-101", "KA-74", "KAS-74U", "KA-M", "M16-A2", "M4-A1", "BK-18", "SVAL", "Vikhr", "Deagle", "Derringer", "CR-75", "FX-45", "IJ-70", "Kolt 1911", "Engraved Kolt 1911", "LongHorn", "MK II", "Mlock-91", "P1", "Revolver", "BK-12", "BK-133", "BK-43", "Vaiga", "Bizon", "CR-61 Skorpion", "SG5-K", "USG-45", "LAR", "CR-527", "CR-550 Savanna", "M70 Tundra", "Mosin 91/30", "Pioneer" , "SSG 82" , "VS-89" , "Blaze", "Repeater Carbine", "VSD", "VSS", "DMR", "SK 59/66", "M79" };
	//std::vector<std::string> Listofammonames = { "7.62x39mm Rounds", ".308 WIN Rounds", "Boxed .308 WIN Rounds", "5.45x39mm Rounds", "Boxed 5.45x39mm Rounds", "Boxed 7.92x57mm Mauser Rounds", "Boxed 7.62x54mmR Rounds", "12ga 00 Buckshots", "Boxed 12ga 00 Buckshots", "7.62x54mmR Rounds", ".22 LR Rounds", "Boxed .22 LR Rounds", "7.62x39mm Tracer Rounds", "Boxed 9x19mm Rounds", "9x19mm Rounds", "Boxed .30-06 Rounds", ".30-06 Rounds", "5.56x45mm Rounds", "7.62x54mmR Tracer Rounds", ".380 ACP Rounds", "12ga Rifled Slugs", "Boxed 12ga Rifled Slugs", "12ga Rubber Slugs", ".356 Rounds", ".357 Rounds", "Boxed .380 ACP Rounds" };
	//std::vector<std::string> ListofBuildingItems = { "Lock Pick", "Plastic Explosive", "Military Crate", "Chainsaw Fuel Bottle", "Burlap Sack", "Nokia 3310", "Plastic Bottle", "Sharpening Stone", "Nails", "Boxed Nails", "Scrap Metal", "Sheet Metal", "Mortar", "Sulphur Powder", "Charcoal Powder", "Gunpowder", "Chainsaw", "Vehicle Cover" };
	//std::vector<std::string> ListofIgnoreItems = { "Wood Wall", "Cereals", "Small Stone", "Field Mushroom", "Shovel", "Firefighter Axe", "Machete", "Wire Mesh Barrier", "Long Stick", "Guts", "Steak Knife", "Salty Sticks", "Flashlight", "Jerrycan", "Netting", "Medical Thermometer", "Charcoal Tablets", "Crude Machete", "", "Wood Floor", "Wood Door", "Power Generator", "Fishing Hook", "Oyster Mushroom", "CivSedanHood_BlackRust", "Apple", "Wood Garage Door", "Cinder Garage Door", "Cannabis Workbench", "Wood Third Wall", "Small Wood Door", "Water Cooler", "Barrel", "Fireplace", "Workbench", "Rifle Locker", "Small Cinder Door", "Plastic Barrel", "Tools Cabinet", "Wood_Shed", "Large Case", "Dustbin", "Dresser", "Wood Storage", "Canopy Tent", "Wood Walkway", "Cinder Window Wall", "Cinder Door", "Wooden Stairs", "Wood Window Wall", "Wood Floor WDoor", "Spear", "Stone", "Stone Knife", "Sickle", "Cleaver", "SK 59/66 Bayonet", "Baseball Bat", "Lead Pipe", "Tire Iron", "Mace", "Meat Tenderizer", "Pipe Wrench", "Brass Knuckles", "Broom", "Farming Hoe", "Paper", "Glow Stick", "Gas Stove", "Cooking Pot", "Campfire Tripod", "Frying Pan", "Ada 4x4 Trunk Door","Ada 4x4 Left Door","Ada 4x4 Right Door", "Olga 24 Rear Right Door", "Olga 24 Rear Left Door", "Olga 24 Front Right Door", "Olga 24 Front Left Door", "Boxed Headlight Bulb", "9V Battery", "Lumber Pile", "Camo Net", "Flare", "Kitchen Timer", "Fence", "Watchtower", "Tarp Shelter", "Fire Barrel", "Packed Pepper Seeds", "Potato", "M3S Right Door", "M3S Left Door", "Human Steak", "Wooden Crate", "Pliers", "Alarm Clock", "Wooden Stick", "Bones", "Construction Light", "Pitchfork", "Garden Lime", "Car Radiator", "Kitchen Knife", "Packed Tomato Seeds", "Spark Plug", "Flag", "Barricade", "Wooden Log", "Hunting Knife", "Kukri", "Pepper", "Fat", "Car Battery", "", "", "" };

	//std::vector<std::string> testingentitytypenames = { "Weapon" };


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


		//bool specialItemAmmo = false;
		//bool specialItemGuns = false;
		//bool specialItemBuilding = false;
		//bool specialItemIgnore = false;

		/*if (item->isGroundItem()) {
			auto bestName2 = item->EntityTypePtr->getBestString();
			if (bestName2 && bestName2->value) {
				// If the player's name is in the list of special names, mark them as special
				//specialItemGuns = std::find(Listofgunnames.begin(), Listofgunnames.end(), bestName2->value) != Listofgunnames.end();
				//specialItemBuilding = std::find(ListofBuildingItems.begin(), ListofBuildingItems.end(), bestName2->value) != ListofBuildingItems.end();
				//specialItemIgnore = std::find(ListofIgnoreItems.begin(), ListofIgnoreItems.end(), bestName2->value) != ListofIgnoreItems.end();
				//specialItemAmmo = std::find(Listofammonames.begin(), Listofammonames.end(), bestName2->value) != Listofammonames.end();
			}
		}*/
		
		if (item->isPlayer()) {
			textCol = IM_COL32(0, 255, 255, 255);
			postFix = " (Dead)";
		} else if (item->isAnimal()) {
			textCol = IM_COL32(0, 255, 0, 100);
			postFix = " (Dead)";
		} else if (item->isGroundItem()) {
			textCol = IM_COL32(255, 255, 255, 200);
			
		}
		else
		{
			continue;
		}

		auto screenPos = WorldToRadar(curMap, mTransform, item->FutureVisualStatePtr->position);

		auto bestName = item->EntityTypePtr->getBestString();

		if (bestName) {
			drawBlip(screenPos, 3, textCol, 11, 1, { bestName->value + postFix });
		}

	}
}

void DayZ::RadarAdapter::drawAliveEntities(DayZ::Camera* camera, DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform, const std::vector<std::shared_ptr<DayZ::Entity>>& entities, Scoreboard* scoreboard)
{
	//std::ifstream myfile ("config.txt");
	//std::string mystring;
	///std::cout << "test" << mystring << std::endl;


	std::vector<std::string> MainPlayerPlayingNames = { "LOCALPLAYERNAME1", "LOCALPLAYERNAME2" };

	std::vector<std::string> specialPlayerNames = { "Friend1", "Friend2", "Friend3", "Friend4", "Friend5", "Friend6" };

	for (auto ent : entities) {
		if (!ent->isValid())
			continue;
		if (ent->isDead)
			continue;
		ImU32 blipColor;
		float blipSize;

		bool ismainplayPlayer = false;
		bool isSpecialPlayer = false;
		// Check for special player names

		std::vector<std::string> infoText;

		if (ent->isPlayer()) {
			auto ident = ent->getPlayerIdentity(scoreboard);
			if (ident && ident->PlayerName) {
				// If the player's name is in the list of special names, mark them as special
				ismainplayPlayer = std::find(MainPlayerPlayingNames.begin(), MainPlayerPlayingNames.end(), ident->PlayerName->value) != MainPlayerPlayingNames.end();
				isSpecialPlayer = std::find(specialPlayerNames.begin(), specialPlayerNames.end(), ident->PlayerName->value) != specialPlayerNames.end();
			}
		}


		if (ismainplayPlayer) {
			blipColor = IM_COL32(255, 0, 255, 255); // Using Lime Green for special players
			blipSize = 5;
		}
		else if (isSpecialPlayer) {
			blipColor = IM_COL32(0, 255, 0, 255);
			blipSize = 5;
		} else if (ent->isPlayer()) {
			blipColor = IM_COL32(255, 0, 0, 255);
			blipSize = 5;
		//} else if (ent->isZombie()) {
		//	blipColor = IM_COL32(255, 255, 0, 130);
		//	blipSize = 3;
		} else if (ent->isAnimal()) {
			blipColor = IM_COL32(0, 255, 0, 130);
			blipSize = 3;
		}
		else {
			continue;
		}

		//auto originPos = ent->FutureVisualStatePtr->position;
		//float entHeight = ent->isAnimal() ? 1 : 1.8;
		//auto topPos = originPos + DMARender::Vector3(0, entHeight, 0);

		//DMARender::Vector2 originW2S, topW2S;
		float dist = camera->InvertedViewTranslation.Dist(ent->FutureVisualStatePtr->position);


		//std::vector<std::string> infoText;
		ImVec2 screenPos = WorldToRadar(curMap, mTransform, ent->FutureVisualStatePtr->position);
		if (ent->isAnimal()) {
			auto entBestStr = ent->EntityTypePtr->getBestString();
			if (entBestStr) {
				auto entName = std::string(ent->EntityTypePtr->getBestString()->value);
				infoText.push_back(entName);
			}
			/// infoText.push_back(std::format("{:.0f}m", dist)); show distance for animals
		}
		if (ent->isPlayer()) {
			auto ident = ent->getPlayerIdentity(scoreboard);
			if (ident && ident->PlayerName && !ismainplayPlayer) {
				infoText.push_back(ident->PlayerName->value);
			}

			if (ent->InventoryPtr->isHandItemValid && !ismainplayPlayer) {
				auto bestHandStr = ent->InventoryPtr->handItem->EntityTypePtr->getBestString();
				if (bestHandStr) {
					infoText.push_back(bestHandStr->value);
				}
				else {
					infoText.push_back("(Unknown)");
				}
			}
			if (!ismainplayPlayer) {
				infoText.push_back(std::format("{:.0f}m", dist));
			}
		}

		if (ent->isPlayer()) {
			drawBlipDirectional(screenPos, blipSize, blipColor, 12, 1, infoText, ent->FutureVisualStatePtr->getRotationCorrected());
		}
		else {
			drawBlip(screenPos, blipSize, blipColor, 11, 1, infoText);

		}
	}

}

DayZ::RadarAdapter::RadarAdapter(DayZ::MemoryUpdater* memUpdater)
{
	this->memUpdater = memUpdater;
}

void DayZ::RadarAdapter::DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform)
{
	auto camera = memUpdater->getCamera();
	ImGui::PushFont(radarFont);
	drawLoot(curMap, mTransform, memUpdater->getSlowEntityTable()->resolvedEntities);
	drawLoot(curMap, mTransform, memUpdater->getItemTable()->resolvedEntities);
	drawLoot(curMap, mTransform, memUpdater->getNearEntityTable()->resolvedEntities);
	drawLoot(curMap, mTransform, memUpdater->getFarEntityTable()->resolvedEntities);
	drawAliveEntities(camera.get(), curMap, mTransform, memUpdater->getFarEntityTable()->resolvedEntities, memUpdater->getScoreboard().get());
	drawAliveEntities(camera.get(), curMap, mTransform, memUpdater->getNearEntityTable()->resolvedEntities, memUpdater->getScoreboard().get());
	ImGui::PopFont();
}

void DayZ::RadarAdapter::createFonts()
{
		ImFontConfig config;
		config.OversampleH = 3;
		config.OversampleV = 1;
		config.GlyphExtraSpacing.x = 1.0f;
		radarFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 32, &config);
		//radarFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 32, &config);
}
