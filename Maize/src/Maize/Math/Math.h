#pragma once

namespace Maize {

	class Math
	{
	public:
		static float Rad2Deg() { return 180.0f / PI(); }
		static float Deg2Rad() { return PI() / 180.0f; }

		static float PI() { return 3.14159265358979323846f; }
		static float Infinity() { return std::numeric_limits<float>::infinity(); }
		static float NegativeInfinity() { return -std::numeric_limits<float>::infinity(); }
		static float Epsilon() { return std::numeric_limits<float>::epsilon(); }

		static float Abs(float value) { return std::abs(value); }
		static int32_t Abs(int32_t value) { return std::abs(value); }
		static float Sqrt(float value) { return std::sqrt(value); }
		static float Pow(float base, float exponent) { return std::pow(base, exponent); }

		static float Sin(float angle) { return std::sin(angle); }
		static float Cos(float angle) { return std::cos(angle); }
		static float Tan(float angle) { return std::tan(angle); }
		static float Asin(float value) { return std::asin(value); }
		static float Acos(float value) { return std::acos(value); }
		static float Atan(float value) { return std::atan(value); }
		static float Atan2(float y, float x) { return std::atan2(y, x); }
		static float Fmod(float x, float y) { return std::fmod(x, y); }

		static float Min(float a, float b) { return std::min(a, b); }
		static int32_t Min(int32_t a, int32_t b) { return std::min(a, b); }
		static float Max(float a, float b) { return std::max(a, b); }
		static int32_t Max(int32_t a, int32_t b) { return std::max(a, b); }

		static float Lerp(float a, float b, float t) { return a + (b - a) * t; }
		static float Clamp(float value, float min, float max) { return std::max(min, std::min(value, max)); }
		static int32_t Clamp(int32_t value, int32_t min, int32_t max) { return std::max(min, std::min(value, max)); }

		static bool Approximately(float a, float b, float epsilon = Epsilon()) { return Abs(a - b) < epsilon; }

		static float Sign(float value) { return (value > 0.0f) ? 1.0f : ((value < 0.0f) ? -1.0f : 0.0f); }

		static float Ceil(float f) { return std::ceil(f); }
		static int32_t CeilToInt(float f) { return std::ceil(f); }
		static float Floor(float f) { return std::floor(f); }
		static int32_t FloorToInt(float f) { return std::floor(f); }
		static float Round(float value) { return std::round(value); }
		static int32_t RoundToInt(float value) { return static_cast<int32_t>(std::round(value)); }
	};

} // Maize
