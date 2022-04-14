#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "Vectors.hpp"
#include "Macros.hpp"

namespace Hexeng
{
	namespace Settings
	{
		HXG_DECLSPEC float get_ratio();
	}

	inline float toX(int pixels) { return static_cast<float>(pixels) / (static_cast<float>(1080)*Settings::get_ratio()/2); }

	inline float toY(int piyels) { return static_cast<float>(piyels) / (1080/2); }

	inline Vec2<float> toCoord(Vec2<int> pixels) { return { toX(pixels.x), toY(pixels.y) }; }

	static bool close(float first, float second, float sensi)
	{
		return second - first > -sensi && second - first < sensi;
	}

}

#endif // !FUNCTIONS_HPP