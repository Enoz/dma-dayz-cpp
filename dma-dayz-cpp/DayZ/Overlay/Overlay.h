#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include "../DayZMem/DayZMem.h"
#include "../Structs/Vectors.h"

class Overlay {
	void threadWorker();
	DayZ::Mem* game;
	std::thread thread;
	DayZ::Vector3 WorldToScreen(DayZ::Camera* camera, DayZ::Vector3 position);
	sf::Font espFont;
	void debugDraw(sf::RenderWindow* window, std::vector<DayZ::Entity*>* entities, DayZ::Camera* camera);

public:
	Overlay(DayZ::Mem* game);
	void run();
	void stop();
};