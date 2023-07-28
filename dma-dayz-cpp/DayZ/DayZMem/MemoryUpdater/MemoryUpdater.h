#pragma once
#include "../DayZMem.h"
#include "../../Structs/WorldNoLists.h"
#include <thread>
#include <mutex>
#include <chrono>
namespace DayZ {
	class MemoryUpdater {
		bool threadRunning = false;

		QWORD worldAddress;
		QWORD cameraAddress;

		DayZ::Mem* mem;
		std::mutex slowMutex;
		std::mutex itemMutex;
		std::mutex farMutex;
		std::mutex nearMutex;
		std::mutex scoreboardMutex;

		std::thread slowThread;
		std::thread itemThread;
		std::thread farThread;
		std::thread nearThread;
		std::thread scoreboardThread;
		std::shared_ptr<EntityTable> NearEntityTable;
		std::shared_ptr<EntityTable> FarEntityTable;
		std::shared_ptr<EntityTableSlowItem> SlowEntityTable;
		std::shared_ptr<EntityTableSlowItem> ItemTable;
		std::shared_ptr<DayZ::Scoreboard> scoreboard;

		void scoreboardWorker();
		void nearTableWorker();
		void farTableWorker();
		void slowTableWorker();
		void itemTableWorker();
		DayZ::WorldNoLists getWNL();

	public:
		MemoryUpdater(DayZ::Mem* mem);
		void beginUpdateLoop();
		void endUpdateLoop();
		std::shared_ptr<EntityTable> getNearEntityTable();
		std::shared_ptr<EntityTable> getFarEntityTable();
		std::shared_ptr<EntityTableSlowItem> getSlowEntityTable();
		std::shared_ptr<EntityTableSlowItem> getItemTable();
		std::shared_ptr<DayZ::Scoreboard> getScoreboard();
		std::shared_ptr<DayZ::Camera> getCamera();


	};
}
