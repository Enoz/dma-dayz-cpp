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

		Vector3 operator-(const Vector3& vec) {
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}

		float Dot(const Vector3& vec) {
			return x * vec.x + y * vec.y + z * vec.z;
		}

		float Dist(const Vector3& vec) {
			return abs(sqrt(pow(this->x - vec.x, 2) + pow(this->y - vec.y, 2) + pow(this->z - vec.z, 2)));
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
