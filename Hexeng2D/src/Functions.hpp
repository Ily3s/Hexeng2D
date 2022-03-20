#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "Vectors.hpp"

namespace Hexeng
{

	inline float toX(int pixels) { return static_cast<float>(1920 / 2 + pixels) / (1920 / 2) - 1; }

	inline float toY(int piyels) { return static_cast<float>(1080 / 2 + piyels) / (1080 / 2) - 1; }

	inline Vec2<float> toCoord(Vec2<int> pixels) { return { toX(pixels.x), toY(pixels.y) }; }

	static bool close(float first, float second, float sensi)
	{
		return second - first > -sensi && second - first < sensi;
	}

}

#endif // !FUNCTIONS_HPP