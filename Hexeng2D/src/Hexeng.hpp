#ifndef HEXENG_HPP
#define HEXENG_HPP

#include <functional>
#include <string>

#include "Macros.hpp"
#include "Vectors.hpp"

struct GLFWwindow;

namespace Hexeng
{

	HXG_DECLSPEC extern GLFWwindow* window;

	HXG_DECLSPEC void game_loop(std::function<void()> pre = nullptr, std::function<void()> post = nullptr);

	namespace Settings
	{

		HXG_DECLSPEC extern bool enable_vsync;
		HXG_DECLSPEC extern std::string window_name;
		HXG_DECLSPEC extern bool fullscreen;
		HXG_DECLSPEC extern Vec2<int> window_size;
		HXG_DECLSPEC float get_ratio();

		HXG_DECLSPEC extern float master_volume;
		HXG_DECLSPEC extern float sound_volume;
		HXG_DECLSPEC extern float music_volume;
	}

	inline float toX(int pixels) { return static_cast<float>(pixels) / (static_cast<float>(1080) * Settings::get_ratio() / 2); }
	inline float toY(int piyels) { return static_cast<float>(piyels) / (1080 / 2); }
	inline Vec2<float> toCoord(Vec2<int> pixels) { return { toX(pixels.x), toY(pixels.y) }; }

	enum class HXG_DECLSPEC Range
	{
		LOCAL,
		GLOBAL
	};

	HXG_DECLSPEC extern float frame_time;

}

#endif