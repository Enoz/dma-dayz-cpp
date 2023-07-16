#include "Overlay.h"

void Overlay::threadWorker()
{
	std::shared_ptr<DayZ::Camera> camera = game->getWorld().WorldPtr->camera;

	sf::RenderWindow overlayWindow(sf::VideoMode(camera->ViewPortSize.x, camera->ViewPortSize.y), "DayZ DMA Overlay");

	while (overlayWindow.isOpen()) {
		sf::Event event;
		while (overlayWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				overlayWindow.close();
		}
	}
}

//https://github.com/uNitx1337/DayZ-External-Tool/blob/master/EnfusionEngine.h
DayZ::Vector3 Overlay::WorldToScreen(DayZ::Camera* camera, DayZ::Vector3 position)
{
	//convert world cords to screen cords :)

	if (!camera)
		return DayZ::Vector3();

	DayZ::Vector3 temp = position - camera->InvertedViewTranslation;

	float x = temp.Dot(camera->InvertedViewRight);
	float y = temp.Dot(camera->InvertedViewUp);
	float z = temp.Dot(camera->InvertedViewForward);

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
