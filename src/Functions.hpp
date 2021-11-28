#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

namespace Hexeng
{

	inline float toX(int pixels) { return (float)(1920 / 2 + pixels) / 1920 * 2 - 1; }

	inline float toY(int piyels) { return (float)(1080 / 2 + piyels) / 1080 * 2 - 1; }

	static bool close(float first, float second, float sensi)
	{
		return second - first > -sensi && second - first < sensi;
	}

}

#endif // !FUNCTIONS_HPP