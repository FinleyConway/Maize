#pragma once

namespace Maize {

	struct Vector2;

	struct Vector2Int
	{
	public:
		Vector2Int() = default;
		Vector2Int(int32_t x, int32_t y);
		Vector2Int(const Vector2Int& other) = default;

		int32_t x = 0;
		int32_t y = 0;

		static Vector2Int Up();
		static Vector2Int Down();
		static Vector2Int Left();
		static Vector2Int Right();
		static Vector2Int One();
		static Vector2Int Zero();

		float Magnitude() const;
		float SqrMagnitude() const;
		void Clamp(Vector2Int min, Vector2Int max);

		static Vector2Int CeilToInt(Vector2 v);
		static Vector2Int FloorToInt(Vector2 v);
		static Vector2Int RoundToInt(Vector2 v);
		static float Distance(Vector2Int a, Vector2Int b);
		static Vector2Int Max(Vector2Int a, Vector2Int b);
		static Vector2Int Min(Vector2Int a, Vector2Int b);

		Vector2Int operator-(const Vector2Int& other) const;
		Vector2Int operator+(const Vector2Int& other) const;
		Vector2Int operator*(const Vector2Int& other) const;
		Vector2Int operator*(int32_t scalar) const;
		Vector2Int operator/(int32_t divisor) const;
		bool operator==(const Vector2Int& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Vector2Int& vector);
	};

}