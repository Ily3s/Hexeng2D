#include "Renderer.hpp"
#include "../Macros.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Presets/Basic.shader"
#include "../Variables.hpp"
#include "Uniform.hpp"
#include "Scene.hpp"
#include "Presets/InitPresets.hpp"

namespace Hexeng::Renderer
{
	
	GLFWwindow* window = nullptr;

	void init()
	{

		HXG_GLFW(if (!glfwInit()) { exit(-1); });

		HXG_GLFW(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4));
		HXG_GLFW(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6));
		HXG_GLFW(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		Vec2<int> window_size{ mode->width, mode->height };

		HXG_GLFW(window = glfwCreateWindow(window_size.x, window_size.y, "Sandbox", 0, NULL));

		HXG_GLFW(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));

		if (!window)
		{
			glfwTerminate();
			exit(-1);
		}

		HXG_GLFW(glfwMakeContextCurrent(window));

		HXG_GLFW(glfwSwapInterval(1));

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			exit(-1);

		std::cout << glGetString(GL_VERSION) << '\n' << std::endl;

		HXG_GL(glEnable(GL_BLEND));
		HXG_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		HXG_GL(glBlendEquation(GL_FUNC_ADD));

		Presets::init();

	}

	void refresh_uniforms()
	{
		HXG_REFRESH_UNIFORM(float);
		HXG_REFRESH_UNIFORM(double);
		HXG_REFRESH_UNIFORM(int);

		HXG_REFRESH_UNIFORM(Vec2<float>);
		HXG_REFRESH_UNIFORM(Vec2<double>);
		HXG_REFRESH_UNIFORM(Vec2<int>);

		HXG_REFRESH_UNIFORM(Vec3<float>);
		HXG_REFRESH_UNIFORM(Vec3<double>);
		HXG_REFRESH_UNIFORM(Vec3<int>);

		HXG_REFRESH_UNIFORM(Vec4<float>);
		HXG_REFRESH_UNIFORM(Vec4<double>);
		HXG_REFRESH_UNIFORM(Vec4<int>);
	}

	void stop()
	{
		for (Layer*& layer : layers)
		{
			for (const auto& mesh : layer->meshes)
			{
				mesh->~Mesh();
				mesh->get_texture()->~Texture();
				mesh->access_shader()->~Shader();
			}
		}

		Presets::stop();

		glfwTerminate();
	}

	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
	{
		va.bind();
		ib.bind();
		shader.bind();
		HXG_GL(glDrawElements(GL_TRIANGLES, ib.get_count(), ib.get_type(), nullptr));
	}

	void clear()
	{
		HXG_GL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void draw(const Layer& layer)
	{
		for (const auto& mesh : layer.meshes)
		{
			mesh->draw();
		}
	}

	void draw_scene(unsigned int scene_parameter)
	{
		for (Layer* lay : scenes[scene_parameter]->layers)
		{
			draw(*lay);
		}
	}

	void draw_current_scene()
	{
		if (scene >= scenes.size())
			return;

		for (Layer* lay : scenes[scene]->layers)
		{
			draw(*lay);
		}
		for (ContextualLayer* cl : contextual_layers)
		{
			if (*cl->context)
				draw(*cl);
		}
	}

}