#pragma once
#include "DMAMemoryManagement/includes.h";
#include "DMARender/includes.h"


namespace DayZ {
	struct Camera : public DMAMem::MemoryObject {
		DMARender::Vector3 InvertedViewRight; //0x0008
		DMARender::Vector3 InvertedViewUp; //0x0014
		DMARender::Vector3 InvertedViewForward; //0x0020
		DMARender::Vector3 InvertedViewTranslation; //0x002C
		DMARender::Vector2 ViewPortSize; //0x0058
		DMARender::Vector3 GetProjectionD1; //0x00D0
		DMARender::Vector3 GetProjectionD2; //0x00DC

		Camera() {
			this->registerOffset(0x8, &InvertedViewRight, sizeof(DMARender::Vector3));
			this->registerOffset(0x14, &InvertedViewUp, sizeof(DMARender::Vector3));
			this->registerOffset(0x20, &InvertedViewForward, sizeof(DMARender::Vector3));
			this->registerOffset(0x2C, &InvertedViewTranslation, sizeof(DMARender::Vector3));
			this->registerOffset(0x58, &ViewPortSize, sizeof(DMARender::Vector2));
			this->registerOffset(0xD0, &GetProjectionD1, sizeof(DMARender::Vector3));
			this->registerOffset(0xDC, &GetProjectionD2, sizeof(DMARender::Vector3));
		}
	};
}