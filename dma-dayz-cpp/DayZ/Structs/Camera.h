#pragma once
#include "DMAMemoryManagement/includes.h";
#include "Vectors.h";

namespace DayZ {
	struct Camera : public DMAMem::MemoryObject {
		Vector3 InvertedViewRight; //0x0008
		Vector3 InvertedViewUp; //0x0014
		Vector3 InvertedViewForward; //0x0020
		Vector3 InvertedViewTranslation; //0x002C
		Vector2 ViewPortSize; //0x0058
		Vector3 GetProjectionD1; //0x00D0
		Vector3 GetProjectionD2; //0x00DC

		Camera() {
			this->registerOffset(0x8, &InvertedViewRight, sizeof(Vector3));
			this->registerOffset(0x14, &InvertedViewUp, sizeof(Vector3));
			this->registerOffset(0x20, &InvertedViewForward, sizeof(Vector3));
			this->registerOffset(0x2C, &InvertedViewTranslation, sizeof(Vector3));
			this->registerOffset(0x58, &ViewPortSize, sizeof(Vector2));
			this->registerOffset(0xD0, &GetProjectionD1, sizeof(Vector3));
			this->registerOffset(0xDC, &GetProjectionD2, sizeof(Vector3));
		}
	};
}