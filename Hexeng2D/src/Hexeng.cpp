#include <algorithm>

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Color.hpp"
#include "Scene.hpp"

namespace Hexeng
{

	GLFWwindow* window = nullptr;

	float frame_time = 0.0f;

	bool exit = false;

	void game_loop(std::function<void()> pre, std::function<void()> post)
	{
		while (!glfwWindowShouldClose(window) && !exit)
		{
			if (pre)
				pre();

			auto start = std::chrono::high_resolution_clock::now();

			Renderer::clear();

			Renderer::refresh_uniforms();

			Renderer::draw_current_scene();

			HXG_GLFW(glfwSwapBuffers(window));

			HXG_GLFW(glfwPollEvents());

			auto end = std::chrono::high_resolution_clock::now();

			frame_time = static_cast<float>(end.time_since_epoch().count() - start.time_since_epoch().count()) / 1000000;

			if (post)
				post();
		}
	}

	namespace Settings
	{
		bool enable_vsync = true;
		std::string window_name = "Game made with Hexeng2D";
		bool fullscreen = false;
		Vec2<int> window_size = { 1280, 720 };

		float get_ratio()
		{
			return static_cast<float>(window_size.x) / window_size.y;
		}

		float master_volume = 1.0f;
		float sound_volume = 1.0f;
		float music_volume = 1.0f;
	}

}