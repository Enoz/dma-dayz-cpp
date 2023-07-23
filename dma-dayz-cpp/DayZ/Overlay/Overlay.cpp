#include "Overlay.h"


void Overlay::threadWorker()
{
	auto initialWorld = game->getWorld();

	//Camera
	std::shared_ptr<DayZ::Camera> camera = initialWorld.WorldPtr->camera;

	//Window + Background
	sf::RenderWindow overlayWindow(sf::VideoMode(initialWorld.WorldPtr->camera->ViewPortSize.x * 2, initialWorld.WorldPtr->camera->ViewPortSize.y * 2), "DayZ DMA Overlay", sf::Style::None);
	sf::RectangleShape rectBackground(sf::Vector2f(initialWorld.WorldPtr->camera->ViewPortSize.x * 2, initialWorld.WorldPtr->camera->ViewPortSize.y * 2));
	rectBackground.setFillColor(sf::Color::Black);


	QWORD cameraAddress = initialWorld.WorldPtr->camera->_remoteAddress;
	QWORD scoreboardAddress = game->getNetworkManager().NetworkClientPtr->scoreboardPtr->_remoteAddress;

	auto scoreBoard = std::shared_ptr<DayZ::Scoreboard>(new DayZ::Scoreboard());
	scoreBoard->resolveObject(game->getVMM(), game->getPid(), scoreboardAddress, NULL);



	unsigned int frame = 0;
	while (overlayWindow.isOpen()) {
		sf::Event event;
		while (overlayWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				overlayWindow.close();
		}

		frame++;
		if (frame > 15000) {
			frame = 0;
		}



		overlayWindow.clear();
		overlayWindow.draw(rectBackground);

		DayZ::World wrld = DayZ::World();
		wrld.resolveObject(game->getVMM(), game->getPid(), initialWorld.WorldPtr->_remoteAddress);


		drawAliveEntities(&overlayWindow, wrld.camera.get(), scoreBoard.get(), wrld.NearEntityTable->resolvedEntities);
		drawAliveEntities(&overlayWindow, wrld.camera.get(), scoreBoard.get(), wrld.FarEntityTable->resolvedEntities);
		drawLoot(&overlayWindow, wrld.camera.get(), wrld.SlowEntityTable->resolvedEntities);
		drawLoot(&overlayWindow, wrld.camera.get(), wrld.ItemTable->resolvedEntities);


		if (frame % 14999 == 0) {
			scoreBoard = std::shared_ptr<DayZ::Scoreboard>(new DayZ::Scoreboard());
			scoreBoard->resolveObject(game->getVMM(), game->getPid(), scoreboardAddress, NULL);
		}
		////debugDraw(&overlayWindow, camera.get());
		//drawAliveEntities(&overlayWindow, camera.get(), scoreBoard.get());
		//drawLoot(&overlayWindow, camera.get());

		overlayWindow.display();


	}
}

void Overlay::debugDraw(sf::RenderWindow* window, DayZ::Camera* camera) {
	auto ents = game->getAllUniqueEntities();
	for (auto ent : ents) {
		DayZ::Vector3 pos = ent->FutureVisualStatePtr->position;
		DayZ::Vector3 screenPos = WorldToScreen(camera, pos);
		if (screenPos.z <= 0 && screenPos.x <= 0 && screenPos.y <= 0) {
			continue;
		}

		if (!ent->isValid())
			continue;

		//if (!ent->isAnimal() && !ent->isPlayer() && !ent->isZombie())
		//	continue;

		float curOffset = 0;

		sf::Text drawText;
		drawText.setFont(espFont);
		drawText.setFillColor(sf::Color::Green);
		drawText.setCharacterSize(12);

		//CleanName
		drawText.setPosition(screenPos.x, screenPos.y + curOffset);
		drawText.setString(ent->EntityTypePtr->CleanName->value);
		window->draw(drawText);
		curOffset += 12;

		//ConfigName
		drawText.setPosition(screenPos.x, screenPos.y + curOffset);
		drawText.setString(ent->EntityTypePtr->ConfigName->value);
		window->draw(drawText);
		curOffset += 12;

		//ModelName
		drawText.setPosition(screenPos.x, screenPos.y + curOffset);
		drawText.setString(ent->EntityTypePtr->ModelName->value);
		window->draw(drawText);
		curOffset += 12;

		//TypeName
		drawText.setPosition(screenPos.x, screenPos.y + curOffset);
		drawText.setString(ent->EntityTypePtr->TypeName->value);
		window->draw(drawText);
		curOffset += 12;

		drawText.setPosition(screenPos.x, screenPos.y + curOffset);
		drawText.setString(std::to_string(ent->EntityTypePtr->CleanName->length));
		window->draw(drawText);
		curOffset += 12;

		if (ent->isPlayer()) {

			drawText.setPosition(screenPos.x, screenPos.y + curOffset);
			drawText.setString("IsPlayer");
			window->draw(drawText);
			curOffset += 12;


			drawText.setPosition(screenPos.x, screenPos.y + curOffset);
			drawText.setString("Dist: " + std::to_string(ent->FutureVisualStatePtr->position.Dist(camera->InvertedViewTranslation)));
			window->draw(drawText);
			curOffset += 12;
		}


	}
}

void Overlay::drawAliveEntities(sf::RenderWindow* window, DayZ::Camera* camera, DayZ::Scoreboard* scoreboard, std::vector<std::shared_ptr<DayZ::Entity>> entities)
{
	for (auto ent : entities) {
		float distance = ent->FutureVisualStatePtr->position.Dist(camera->InvertedViewTranslation);
		if (!ent->isAnimal() && !ent->isPlayer() && !ent->isZombie())
			continue;
		if (ent->isDead)
			continue;

		if (distance < 5.0f)
			continue;
		if (ent->isZombie() && distance > 200.0f)
			continue;


		DayZ::Vector3 pos = ent->FutureVisualStatePtr->position;

		sf::Color drawColor;
		if (ent->isAnimal()) {
			drawColor = sf::Color::Green;
		}
		if (ent->isPlayer()) {
			drawColor = sf::Color::Red;
		}
		if (ent->isZombie()) {
			drawColor = sf::Color::Yellow;
		}


		auto bottomW2S = WorldToScreen(camera, pos);
		auto topW2S = WorldToScreen(camera, DayZ::Vector3(pos.x, pos.y + 1.8, pos.z));

		if (bottomW2S.x == 0 && bottomW2S.y == 0 && bottomW2S.z == 0)
			continue;
		if (topW2S.x == 0 && topW2S.y == 0 && topW2S.z == 0)
			continue;

		
		auto boxWidth = (bottomW2S.y - topW2S.y) / 1.8f;

		float textPadding = 8.0f;
		float textSize = boxWidth;
		if (textSize < 16.0f)
			textSize = 16.0f;
		if (textSize > 24.0f)
			textSize = 24.0f;

		if (ent->isPlayer()) {
			auto ident = ent->getPlayerIdentity(scoreboard);
			if (ident != NULL) {
				drawText(window, DayZ::Vector3(topW2S.x + (boxWidth / 2) + textPadding, topW2S.y, 0), drawColor, textSize, ident->PlayerName->value);
				drawText(window, DayZ::Vector3(topW2S.x + (boxWidth / 2) + textPadding, topW2S.y-textSize, 0), drawColor, textSize, std::to_string((int)floor(distance)));
			}
			auto handItemArmaStr = ent->InventoryPtr->handItem->EntityTypePtr->getBestString();
			if (handItemArmaStr != nullptr) {
				drawText(window, DayZ::Vector3(topW2S.x + (boxWidth / 2) + textPadding, topW2S.y - (textSize*2), 0), drawColor, textSize, ent->InventoryPtr->handItem->EntityTypePtr->getBestString()->value);
			}
			
		}

		if (ent->isAnimal()) {
			drawText(window, DayZ::Vector3(topW2S.x + (boxWidth / 2) + textPadding, topW2S.y, 0), drawColor, textSize, ent->EntityTypePtr->CleanName->value);
		}


		drawBox(window, bottomW2S, topW2S, boxWidth, drawColor);

	}
}

void Overlay::drawLoot(sf::RenderWindow* window, DayZ::Camera* camera, std::vector<std::shared_ptr<DayZ::Entity>> entities)
{
	for (auto ent : entities) {
		if (
			!(ent->isGroundItem()) &&
			!(ent->isPlayer() && ent->isDead) &&
			!(ent->isZombie() && ent->isDead) &&
			!(ent->isAnimal() && ent->isDead)
			) {
			continue;
		}

		sf::Color col;
		float maxDist = 10000;
		std::string postFix = "";
		if (ent->isPlayer()) {
			col = sf::Color::Red;
			maxDist = 2000;
			postFix = "(Dead)";
		}
		if (ent->isAnimal()) {
			col = sf::Color::Green;
			maxDist = 2000;
			postFix = "(Dead)";
		}
		if (ent->isZombie()) {
			col = sf::Color::Yellow;
			maxDist = 50;
			postFix = "(Dead)";
		}
		if (ent->isGroundItem()) {
			col = sf::Color::White;
			maxDist = 2000;
		}

		DayZ::Vector3 screenPos = WorldToScreen(camera, ent->FutureVisualStatePtr->position);
		float dist = camera->InvertedViewTranslation.Dist(ent->FutureVisualStatePtr->position);
		if (dist > maxDist)
			continue;
		if (screenPos.x == 0 && screenPos.y == 0 && screenPos.z == 0)
			continue;
		float size = 100 / dist;
		if (size < 10) {
			size = 10;
		}

		//sf::CircleShape itemDot;
		//itemDot.setRadius(size);
		//itemDot.setPosition(screenPos.x, screenPos.y);
		//itemDot.setFillColor(col);

		std::shared_ptr<DayZ::ArmaString> displayText = ent->EntityTypePtr->getBestString();
		if (displayText != nullptr) {
			drawText(window, DayZ::Vector3(screenPos.x, screenPos.y - size, screenPos.z), col, size, std::string(displayText->value) + " " + postFix);
		}

		//window->draw(itemDot);




	}
}

void Overlay::drawBox(sf::RenderWindow* window, DayZ::Vector3 bottom, DayZ::Vector3 top, float width, sf::Color color)
{

	std::vector<sf::Vector2f> lineOrder {
		sf::Vector2f(top.x + width / 2, top.y),
		sf::Vector2f(bottom.x + width / 2, bottom.y),
		sf::Vector2f(bottom.x - width / 2, bottom.y),
		sf::Vector2f(top.x - width / 2, top.y),
		sf::Vector2f(top.x + width / 2, top.y)
	};
	for (int i = 0; i < lineOrder.size() - 1; i++) {
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = lineOrder[i];
		line[1].position = lineOrder[i + 1];

		line[0].color = color;
		line[1].color = color;

		window->draw(line);
	}


}

void Overlay::drawText(sf::RenderWindow* window, DayZ::Vector3 screenPos, sf::Color color, int size, std::string text)
{
	if (size > 50 || size < 2)
		return;
	sf::Text drawText;
	drawText.setFont(espFont);
	drawText.setFillColor(color);
	drawText.setString(text);
	drawText.setPosition(screenPos.x, screenPos.y);
	drawText.setCharacterSize(size);

	window->draw(drawText);
}

//std::shared_ptr<DayZ::EntityManager> Overlay::getEntityManager()
//{
//	return this->entityManager;
//}
//
//void Overlay::refreshEntityManager()
//{
//	entityManager = std::shared_ptr<DayZ::EntityManager>(new DayZ::EntityManager(game->getVMM(), game->getPid(), game->getAllUniqueEntities()));
//}



//https://github.com/uNitx1337/DayZ-External-Tool/blob/master/EnfusionEngine.h EnfusionEngine::WorldToScreen (Thank You!!)
DayZ::Vector3 Overlay::WorldToScreen(DayZ::Camera* camera, DayZ::Vector3 position)
{
	//convert world cords to screen cords :)

	if (!camera)
		return DayZ::Vector3();

	DayZ::Vector3 temp = DayZ::Vector3(position.x - camera->InvertedViewTranslation.x,
		position.y - camera->InvertedViewTranslation.y,
		position.z - camera->InvertedViewTranslation.z
		);

	float x = temp.Dot(camera->InvertedViewRight);
	float y = temp.Dot(camera->InvertedViewUp);
	float z = temp.Dot(camera->InvertedViewForward);

	if (z < 0.65f) {
		return DayZ::Vector3();
	}

	return DayZ::Vector3(
		camera->ViewPortSize.x * (1 + (x / camera->GetProjectionD1.x / z)),
		camera->ViewPortSize.y * (1 - (y / camera->GetProjectionD2.y / z)),
		z);
}

Overlay::Overlay(DayZ::Mem* game)
{
	this->game = game;
	//this->refreshEntityManager();

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	espFont = font;
}

void Overlay::run()
{
	this->thread = std::thread(&Overlay::threadWorker, this);
}

void Overlay::stop()
{
	this->thread.~thread();
}