#include "Overlay.h"

void Overlay::threadWorker()
{
	std::shared_ptr<DayZ::Camera> camera = game->getWorld().WorldPtr->camera;

	sf::RenderWindow overlayWindow(sf::VideoMode(camera->ViewPortSize.x*2, camera->ViewPortSize.y*2), "DayZ DMA Overlay");
	sf::RectangleShape rectBackground(sf::Vector2f(camera->ViewPortSize.x*2, camera->ViewPortSize.y*2));
	rectBackground.setFillColor(sf::Color::Black);

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
		camera = std::shared_ptr<DayZ::Camera>(new DayZ::Camera(game->getVMM(), game->getPid(), camera->lastRemoteAddressUsed));
		//nearItems = std::shared_ptr<DayZ::EntityTable>(new DayZ::EntityTable(game->getVMM(), game->getPid(), nearItems->lastRemoteAddressUsed));
		//std::shared_ptr<DayZ::EntityTable> nearItems = game->getWorld().WorldPtr->NearEntityTable;
		for (auto ent : nearItems->resolvedEntities) {
			DayZ::Vector3 pos = ent->FutureVisualStatePtr->position;
			DayZ::Vector3 screenPos = WorldToScreen(camera.get(), pos);
			if (screenPos.z <= 0 && screenPos.x <= 0 && screenPos.y <= 0) {
				continue;
			}

			sf::CircleShape entCircle(5.0f);
			entCircle.setFillColor(sf::Color::Green);
			entCircle.setPosition(screenPos.x, screenPos.y);
			overlayWindow.draw(entCircle);
			
		}


		overlayWindow.display();


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
}

void Overlay::run()
{
	this->thread = std::thread(&Overlay::threadWorker, this);
}

void Overlay::stop()
{
	this->thread.~thread();
}
