#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include "../DayZMem/DayZMem.h"
#include "../Structs/Vectors.h"
#include "DMAMemoryManagement/Utils/Utils.h"
#include "../DayZMem/EntityManager/EntityManager.h"

class Overlay {
	void threadWorker();
	DayZ::Mem* game;
	std::thread thread;
	DayZ::Vector3 WorldToScreen(DayZ::Camera* camera, DayZ::Vector3 position);
	sf::Font espFont;
	void debugDraw(sf::RenderWindow* window, DayZ::Camera* camera, std::vector<std::shared_ptr<DayZ::Entity>> entities);
	void drawAliveEntities(sf::RenderWindow* window, DayZ::Camera* camera, DayZ::Scoreboard* scoreboard, std::vector<std::shared_ptr<DayZ::Entity>> entities);
	void drawLoot(sf::RenderWindow* window, DayZ::Camera* camera, std::vector<std::shared_ptr<DayZ::Entity>> entities);
	void drawBox(sf::RenderWindow* window, DayZ::Vector3 bottom, DayZ::Vector3 top, float width, sf::Color color);
	void drawText(sf::RenderWindow* window, DayZ::Vector3 screenPos, sf::Color color, int size, std::string text);

public:
	Overlay(DayZ::Mem* game);
	void run();
	void stop();
};