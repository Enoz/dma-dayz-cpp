#pragma once
#include "DMARender/includes.h"
#include "../MemoryUpdater/MemoryUpdater.h"
namespace DayZ {
	class RadarAdapter : public DMARender::IRadar {

		DayZ::MemoryUpdater* memUpdater;
	public:
		RadarAdapter(DayZ::MemoryUpdater* memUpdater);


		// Inherited via IRadar
		virtual void DrawOverlay(DMARender::IGameMap* curMap, const DMARender::MapTransform& mTransform) override;

	};
}