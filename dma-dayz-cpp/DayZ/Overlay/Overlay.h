#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include "../DayZMem/DayZMem.h"
#include "DMARender/includes.h"
#include "DMAMemoryManagement/Utils/Utils.h"
#include "../DayZMem/EntityManager/EntityManager.h"

class Overlay {
	void threadWorker();
	DayZ::Mem* game;
	std::thread thread;
	DMARender::Vector3 WorldToScreen(DayZ::Camera* camera, DMARender::Vector3 position);
	sf::Font espFont;
	void debugDraw(sf::RenderWindow* window, DayZ::Camera* camera, std::vector<std::shared_ptr<DayZ::Entity>> entities);
	void drawAliveEntities(sf::RenderWindow* window, DayZ::Camera* camera, DayZ::Scoreboard* scoreboard, std::vector<std::shared_ptr<DayZ::Entity>> entities);
	void drawLoot(sf::RenderWindow* window, DayZ::Camera* camera, std::vector<std::shared_ptr<DayZ::Entity>> entities);
	void drawBox(sf::RenderWindow* window, DMARender::Vector3 bottom, DMARender::Vector3 top, float width, sf::Color color);
	void drawText(sf::RenderWindow* window, DMARender::Vector3 screenPos, sf::Color color, int size, std::string text);

public:
	Overlay(DayZ::Mem* game);
	void run();
	void stop();
};