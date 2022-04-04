#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"

namespace Hexeng
{

	GLFWwindow* window = nullptr;

	int scene_id = 0;

	void game_loop(std::function<void()> pre, std::function<void()> post)
	{
		while (!glfwWindowShouldClose(window))
		{
			if (pre)
				pre();

			Renderer::clear();

			Renderer::refresh_uniforms();

			Renderer::draw_current_scene();

			HXG_GLFW(glfwSwapBuffers(window));

			HXG_GLFW(glfwPollEvents());

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
	}

}