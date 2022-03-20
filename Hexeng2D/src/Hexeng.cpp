#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"

namespace Hexeng
{

	void game_loop(std::function<void()> pre, std::function<void()> post)
	{
		while (!glfwWindowShouldClose(Renderer::window))
		{
			if (pre)
				pre();

			Renderer::clear();

			Renderer::refresh_uniforms();

			Renderer::draw_current_scene();

			HXG_GLFW(glfwSwapBuffers(Renderer::window));

			HXG_GLFW(glfwPollEvents());

			if (post)
				post();
		}
	}

}