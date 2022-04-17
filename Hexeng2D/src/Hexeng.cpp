#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Color.hpp"
#include "Scene.hpp"

namespace Hexeng
{

	GLFWwindow* window = nullptr;

	void game_loop(std::function<void()> pre, std::function<void()> post)
	{
		Renderer::pending_actions.push_back([]()
			{
				for (auto& [id, scene] : scenes)
					scene->unload();

				scenes[scene_id]->load();
				for (auto& layer : Renderer::global_layers)
					layer->load();
				for (auto& cl : Renderer::global_contextual_layers)
					cl->load();
			});

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

		float get_ratio()
		{
			return static_cast<float>(window_size.x) / window_size.y;
		}
	}

}