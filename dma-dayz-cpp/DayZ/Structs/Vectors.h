#pragma once
namespace DayZ {
	struct Vector3 {
		float x;
		float y;
		float z;
		Vector3() {
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}
		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
	};

	struct Vector2 {
		float x;
		float y;
		Vector2() {
			this->x = 0.0f;
			this->y = 0.0f;
		}
		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}
	};
}
