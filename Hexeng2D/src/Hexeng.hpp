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

	namespace Settings
	{

		HXG_DECLSPEC extern bool enable_vsync;
		HXG_DECLSPEC extern std::string window_name;
		HXG_DECLSPEC extern bool fullscreen;
		HXG_DECLSPEC extern Vec2<int> window_size;

	}

}

#endif