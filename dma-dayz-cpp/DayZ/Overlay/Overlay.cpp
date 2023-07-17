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




		//Combine Entity Tables
		auto combinedEntities = std::vector<DayZ::Entity*>();
		//for (const auto& ent : nearEntities->resolvedEntities)
		//	combinedEntities.push_back(ent.get());
		//for (const auto& ent : farEntities->resolvedEntities)
		//	combinedEntities.push_back(ent.get());
		for (const auto& ent : slowEntities->resolvedEntities)
			combinedEntities.push_back(ent.get());
		//for (const auto& ent : itemEntities->resolvedEntities)
		//	combinedEntities.push_back(ent.get());

		debugDraw(&overlayWindow, &combinedEntities, camera.get());


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

		//sf::CircleShape entCircle(5.0f);
		//entCircle.setFillColor(sf::Color::Green);
		//entCircle.setPosition(screenPos.x, screenPos.y);

		sf::Text text;
		text.setFont(espFont);
		text.setCharacterSize(16);
		text.setFillColor(sf::Color::Green);
		text.setPosition(screenPos.x, screenPos.y);
		text.setString(ent->EntityTypePtr->ConfigName->value);
		window->draw(text);
	}
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