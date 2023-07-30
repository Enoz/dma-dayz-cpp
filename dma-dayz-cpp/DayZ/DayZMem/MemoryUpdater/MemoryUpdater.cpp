#include "MemoryUpdater.h"

void DayZ::MemoryUpdater::scoreboardWorker()
{
	while (threadRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		auto newScoreboard = this->mem->getNetworkManager().NetworkClientPtr->scoreboardPtr;
		const std::lock_guard<std::mutex> lock(scoreboardMutex);
		this->scoreboard = newScoreboard;
	}
}

void DayZ::MemoryUpdater::nearTableWorker()
{
	while (threadRunning) {
		auto wnl = getWNL();
		auto newNearEntity = std::shared_ptr<EntityTable>(new EntityTable(wnl.NearEntityTableCount));
		newNearEntity->resolveObject(mem->getVMM(), mem->getPid(), wnl.NearTableAddress);
		const std::lock_guard<std::mutex> lock(nearMutex);
		this->NearEntityTable = newNearEntity;
	}
}

void DayZ::MemoryUpdater::farTableWorker()
{
	while (threadRunning) {
		auto wnl = getWNL();
		auto newFarEntity = std::shared_ptr<EntityTable>(new EntityTable(wnl.FarEntityTableCount));
		newFarEntity->resolveObject(mem->getVMM(), mem->getPid(), wnl.FarTableAddress);
		const std::lock_guard<std::mutex> lock(farMutex);
		this->FarEntityTable = newFarEntity;
	}
}

void DayZ::MemoryUpdater::slowTableWorker()
{
	while (threadRunning) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		auto wnl = getWNL();
		auto newSlowEntity = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(wnl.SlowEntityCountAlloc, wnl.SlowEntityValidCount));
		newSlowEntity->resolveObject(mem->getVMM(), mem->getPid(), wnl.SlowTableAddress);
		const std::lock_guard<std::mutex> lock(slowMutex);
		this->SlowEntityTable = newSlowEntity;
	}
}

void DayZ::MemoryUpdater::itemTableWorker()
{
	while (threadRunning) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		auto wnl = getWNL();
		auto newItemEntity = std::shared_ptr<EntityTableSlowItem>(new EntityTableSlowItem(wnl.ItemTableCountAlloc, wnl.ItemTableCount));
		newItemEntity->resolveObject(mem->getVMM(), mem->getPid(), wnl.ItemTableAddress);
		const std::lock_guard<std::mutex> lock(itemMutex);
		this->ItemTable = newItemEntity;
	}
}

DayZ::WorldNoLists DayZ::MemoryUpdater::getWNL()
{
	WorldNoLists wnl = WorldNoLists();
	wnl.resolveObject(mem->getVMM(), mem->getPid(), worldAddress);
	return wnl;
}

DayZ::MemoryUpdater::MemoryUpdater(DayZ::Mem* mem)
{
	this->mem = mem;
	auto wrld = this->mem->getWorld().WorldPtr;
	this->worldAddress = wrld->_remoteAddress;
	this->cameraAddress = wrld->camera->_remoteAddress;
	//Seed to prevent race conditions
	this->NearEntityTable = wrld->NearEntityTable;
	this->FarEntityTable = wrld->FarEntityTable;
	this->SlowEntityTable = wrld->SlowEntityTable;
	this->ItemTable = wrld->ItemTable;
	this->scoreboard = this->mem->getNetworkManager().NetworkClientPtr->scoreboardPtr;
}

void DayZ::MemoryUpdater::beginUpdateLoop()
{
	this->threadRunning = true;
	this->nearThread = std::thread(&MemoryUpdater::nearTableWorker, this);
	this->farThread = std::thread(&MemoryUpdater::farTableWorker, this);
	this->slowThread = std::thread(&MemoryUpdater::slowTableWorker, this);
	this->itemThread = std::thread(&MemoryUpdater::itemTableWorker, this);
	this->scoreboardThread = std::thread(&MemoryUpdater::scoreboardWorker, this);

}

void DayZ::MemoryUpdater::endUpdateLoop()
{
	this->threadRunning = false;
	if(this->nearThread.joinable())
		this->nearThread.join();
	if (this->farThread.joinable())
		this->farThread.join();
	if (this->slowThread.joinable())
		this->slowThread.join();
	if (this->itemThread.joinable())
		this->itemThread.join();
	if (this->scoreboardThread.joinable())
		this->scoreboardThread.join();
}

std::shared_ptr<DayZ::EntityTable> DayZ::MemoryUpdater::getNearEntityTable()
{
	const std::lock_guard<std::mutex> lock(nearMutex);
	return this->NearEntityTable;
}

std::shared_ptr<DayZ::EntityTable> DayZ::MemoryUpdater::getFarEntityTable()
{
	const std::lock_guard<std::mutex> lock(farMutex);
	return this->FarEntityTable;
}

std::shared_ptr<DayZ::EntityTableSlowItem> DayZ::MemoryUpdater::getSlowEntityTable()
{
	const std::lock_guard<std::mutex> lock(slowMutex);
	return this->SlowEntityTable;
}

std::shared_ptr<DayZ::EntityTableSlowItem> DayZ::MemoryUpdater::getItemTable()
{
	const std::lock_guard<std::mutex> lock(itemMutex);
	return this->ItemTable;
}

std::shared_ptr<DayZ::Scoreboard> DayZ::MemoryUpdater::getScoreboard()
{
	const std::lock_guard<std::mutex> lock(scoreboardMutex);
	return this->scoreboard;
}

std::shared_ptr<DayZ::Camera> DayZ::MemoryUpdater::getCamera()
{
	std::shared_ptr<DayZ::Camera> camera(new DayZ::Camera());
	camera->resolveObject(mem->getVMM(), mem->getPid(), cameraAddress);
	return camera;
}
