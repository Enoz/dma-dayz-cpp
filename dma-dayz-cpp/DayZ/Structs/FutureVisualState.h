#pragma once
#include "DMAMemoryManagement/includes.h";
#include "DMARender/includes.h"
#include <math.h>
#include <numbers>

namespace DayZ {
	struct FutureVisualState : public DMAMem::MemoryObject {
		DMARender::Vector3 position;
		float dirX = 0.0f;
		float dirY = 0.0f;


		FutureVisualState() {
			this->registerOffset(0x2c, &position, sizeof(DMARender::Vector3));
			this->registerOffset(0x20, &dirX, sizeof(float));
			this->registerOffset(0x28, &dirY, sizeof(float));
		}

		float getRotation() {
			return (float)(atan2(dirY, dirX) * ((180.0f / std::numbers::pi)));
		}

		float getRotationCorrected() {
			float rot = getRotation();
			//Rotate by 90 degrees for radar
			rot += 90;
			return rot;
		}


	};
}