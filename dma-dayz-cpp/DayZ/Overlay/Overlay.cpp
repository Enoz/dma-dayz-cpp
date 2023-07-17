#include "Overlay.h"


void Overlay::threadWorker()
{
	auto initialWorld = game->getWorld();

	//Camera
	std::shared_ptr<DayZ::Camera> initCamera = initialWorld.WorldPtr->camera;

	//Window + Background
	sf::RenderWindow overlayWindow(sf::VideoMode(initialWorld.WorldPtr->camera->ViewPortSize.x * 2, initialWorld.WorldPtr->camera->ViewPortSize.y * 2), "DayZ DMA Overlay", sf::Style::None);
	sf::RectangleShape rectBackground(sf::Vector2f(initialWorld.WorldPtr->camera->ViewPortSize.x * 2, initialWorld.WorldPtr->camera->ViewPortSize.y * 2));
	rectBackground.setFillColor(sf::Color::Black);


	QWORD cameraAddress = initialWorld.WorldPtr->camera->_lastAddressUsed;
	QWORD nearAddress = initialWorld.WorldPtr->NearEntityTable->_lastAddressUsed;
	QWORD farAddress = initialWorld.WorldPtr->FarEntityTable->_lastAddressUsed;
	QWORD slowAddress = initialWorld.WorldPtr->SlowEntityTable->_lastAddressUsed;
	QWORD itemAddress = initialWorld.WorldPtr->ItemTable->_lastAddressUsed;
	QWORD scoreboardAddress = game->getNetworkManager().NetworkClientPtr->scoreboardPtr->_lastAddressUsed;

	std::shared_ptr<DayZ::EntityTable> nearItems = game->getWorld().WorldPtr->ItemTable;
	while (overlayWindow.isOpen()) {
		sf::Event event;
		while (overlayWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				overlayWindow.close();
		}
		overlayWindow.clear();
		overlayWindow.draw(rectBackground);

		auto camera = std::shared_ptr<DayZ::Camera>(new DayZ::Camera());
		camera->resolveObject(game->getVMM(), game->getPid(), cameraAddress);

		auto nearEntities = std::shared_ptr<DayZ::EntityTable>(new DayZ::EntityTable());
		nearEntities->resolveObject(game->getVMM(), game->getPid(), nearAddress);

		auto farEntities = std::shared_ptr<DayZ::EntityTable>(new DayZ::EntityTable());
		farEntities->resolveObject(game->getVMM(), game->getPid(), farAddress, NULL);

		auto slowEntities = std::shared_ptr<DayZ::EntityTable>(new DayZ::EntityTable());
		slowEntities->resolveObject(game->getVMM(), game->getPid(), slowAddress, NULL);

		auto itemEntities = std::shared_ptr<DayZ::EntityTable>(new DayZ::EntityTable());
		itemEntities->resolveObject(game->getVMM(), game->getPid(), itemAddress, NULL);

		auto scoreBoard = std::shared_ptr<DayZ::Scoreboard>(new DayZ::Scoreboard());
		scoreBoard->resolveObject(game->getVMM(), game->getPid(), scoreboardAddress, NULL);




		//Combine Entity Tables
		auto combinedEntities = std::vector<DayZ::Entity*>();
		for (const auto& ent : nearEntities->resolvedEntities)
			combinedEntities.push_back(ent.get());
		for (const auto& ent : farEntities->resolvedEntities)
			combinedEntities.push_back(ent.get());
		for (const auto& ent : slowEntities->resolvedEntities)
			combinedEntities.push_back(ent.get());
		for (const auto& ent : itemEntities->resolvedEntities)
			combinedEntities.push_back(ent.get());

		//debugDraw(&overlayWindow, &combinedEntities, camera.get());
		drawAliveEntities(&overlayWindow, &combinedEntities, camera.get(), scoreBoard.get());


		overlayWindow.display();


	}
}

void Overlay::debugDraw(sf::RenderWindow* window, std::vector<DayZ::Entity*>* entities, DayZ::Camera* camera) {
	for (auto ent : *entities) {
		DayZ::Vector3 pos = ent->FutureVisualStatePtr->position;
		DayZ::Vector3 screenPos = WorldToScreen(camera, pos);
		if (screenPos.z <= 0 && screenPos.x <= 0 && screenPos.y <= 0) {
			continue;
		}

		if (!ent->isAnimal() && !ent->isPlayer() && !ent->isZombie())
			continue;

		//sf::CircleShape entCircle(5.0f);
		//entCircle.setFillColor(sf::Color::Green);
		//entCircle.setPosition(screenPos.x, screenPos.y);

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

		if (ent->isPlayer() || ent->isZombie()) {

			drawText.setPosition(screenPos.x, screenPos.y + curOffset);
			drawText.setString("IsPlayer");
			window->draw(drawText);
			curOffset += 12;


			drawText.setPosition(screenPos.x, screenPos.y + curOffset);
			drawText.setString("Dist: " + std::to_string(ent->FutureVisualStatePtr->position.Dist(camera->InvertedViewTranslation)));
			window->draw(drawText);
			curOffset += 12;

			drawText.setPosition(screenPos.x, screenPos.y + curOffset);
			drawText.setString("isDead: " + std::to_string(ent->isDead));
			window->draw(drawText);
			curOffset += 12;


		}


	}
}

void Overlay::drawAliveEntities(sf::RenderWindow* window, std::vector<DayZ::Entity*>* entities, DayZ::Camera* camera, DayZ::Scoreboard* scoreboard)
{
	for (auto ent : *entities) {
		if (ent->isDead)
			continue;
		if (!ent->isAnimal() && !ent->isPlayer() && !ent->isZombie())
			continue;

		float distance = ent->FutureVisualStatePtr->position.Dist(camera->InvertedViewTranslation);

		if (distance < 5.0f)
			continue;


		DayZ::Vector3 pos = ent->FutureVisualStatePtr->position;
		DayZ::Vector3 screenPos = WorldToScreen(camera, pos);

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
		auto boxWidth = (bottomW2S.y - topW2S.y) / 1.8f;

		float textPadding = 8.0f;
		float textSize = boxWidth;
		if (textSize < 15.0f)
			textSize = 15.0f;
		if (textSize > 48.0f)
			textSize = 48.0f;

		if (ent->isPlayer()) {
			auto ident = ent->getPlayerIdentity(scoreboard);
			if (ident != NULL) {
				drawText(window, DayZ::Vector3(topW2S.x + (boxWidth / 2) + textPadding, topW2S.y, 0), drawColor, textSize, ident->PlayerName->value);
			}
		}

		if (ent->isAnimal()) {
			drawText(window, DayZ::Vector3(topW2S.x + (boxWidth / 2) + textPadding, topW2S.y, 0), drawColor, textSize, ent->EntityTypePtr->CleanName->value);
		}


		drawBox(window, bottomW2S, topW2S, boxWidth, drawColor);

	}
}

void Overlay::drawLoot(sf::RenderWindow* window, std::vector<DayZ::Entity*>* entities, DayZ::Camera* camera)
{

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