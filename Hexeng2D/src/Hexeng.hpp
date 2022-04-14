#ifndef HEXENG_HPP
#define HEXENG_CPP

#include <functional>
#include <string>

#include "Macros.hpp"
#include "Vectors.hpp"

struct GLFWwindow;

namespace Hexeng
{

	HXG_DECLSPEC extern GLFWwindow* window;

	HXG_DECLSPEC void game_loop(std::function<void()> pre = nullptr, std::function<void()> post = nullptr);

	HXG_DECLSPEC extern int scene_id;

	namespace Settings
	{

		HXG_DECLSPEC extern bool enable_vsync;
		HXG_DECLSPEC extern std::string window_name;
		HXG_DECLSPEC extern bool fullscreen;
		HXG_DECLSPEC extern Vec2<int> window_size;
		HXG_DECLSPEC float get_ratio();

	}

	inline float toX(int pixels) { return static_cast<float>(pixels) / (static_cast<float>(1080) * Settings::get_ratio() / 2); }
	inline float toY(int piyels) { return static_cast<float>(piyels) / (1080 / 2); }
	inline Vec2<float> toCoord(Vec2<int> pixels) { return { toX(pixels.x), toY(pixels.y) }; }

}

#endif