#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include "../DayZMem/DayZMem.h"
#include "../Structs/Vectors.h"
#include "../../DMAMemoryManagement/Utils/Utils.h"

class Overlay {
	void threadWorker();
	DayZ::Mem* game;
	std::thread thread;
	DayZ::Vector3 WorldToScreen(DayZ::Camera* camera, DayZ::Vector3 position);
	sf::Font espFont;
	void debugDraw(sf::RenderWindow* window, std::vector<std::shared_ptr<DayZ::Entity>>* entities, DayZ::Camera* camera);
	void drawAliveEntities(sf::RenderWindow* window, std::vector<std::shared_ptr<DayZ::Entity>>* entities, DayZ::Camera* camera, DayZ::Scoreboard* scoreboard);
	void drawLoot(sf::RenderWindow* window, std::vector<std::shared_ptr<DayZ::Entity>>* entities, DayZ::Camera* camera);
	void drawBox(sf::RenderWindow* window, DayZ::Vector3 bottom, DayZ::Vector3 top, float width, sf::Color color);
	void drawText(sf::RenderWindow* window, DayZ::Vector3 screenPos, sf::Color color, int size, std::string text);

public:
	Overlay(DayZ::Mem* game);
	void run();
	void stop();
};