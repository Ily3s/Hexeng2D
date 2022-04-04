#include "Renderer.hpp"
#include "../Macros.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Presets/Basic.shader"
#include "Uniform.hpp"
#include "Scene.hpp"
#include "Presets/InitPresets.hpp"
#include "Camera.hpp"
#include "../Hexeng.hpp"

namespace Hexeng::Renderer
{

	void init()
	{

		HXG_GLFW(if (!glfwInit()) { exit(-1); });

		HXG_GLFW(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4));
		HXG_GLFW(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6));
		HXG_GLFW(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		Vec2<int> screen_size{ mode->width, mode->height };

		if (Settings::fullscreen)
		{
			HXG_GLFW(window = glfwCreateWindow(screen_size.x, screen_size.y, Settings::window_name.c_str(), glfwGetPrimaryMonitor(), NULL));
		}
		else
		{
			HXG_GLFW(window = glfwCreateWindow(Settings::window_size.x, Settings::window_size.y, Settings::window_name.c_str(), 0, NULL));
		}

		HXG_GLFW(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));

		if (!window)
		{
			glfwTerminate();
			exit(-1);
		}

		HXG_GLFW(glfwMakeContextCurrent(window));

		HXG_GLFW(glfwSwapInterval(Settings::enable_vsync));

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			exit(-1);

		std::cout << glGetString(GL_VERSION) << '\n' << std::endl;

		HXG_GL(glEnable(GL_BLEND));
		HXG_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		HXG_GL(glBlendEquation(GL_FUNC_ADD));

		HXG_GL(glLineWidth(2.0f));

		u_transform = { "u_transform", &transform, {} };
		UniformInterface::necessary_uniforms.push_back(&u_transform);

		Camera::init();
		Presets::init();
	}

	std::vector<UniformInterface*> uniform_list;

	void refresh_uniforms()
	{
		for (UniformInterface* ui : uniform_list)
			ui->refresh();
	}

	void stop()
	{
		for (Layer*& layer : layers)
		{
			for (const auto& mesh : layer->meshes)
			{
				mesh->~Mesh();
				if (mesh->get_texture())
					mesh->get_texture()->~Texture();
				mesh->access_shader()->~Shader();
			}
		}

		for (ContextualLayer*& layer : contextual_layers)
		{
			for (const auto& mesh : layer->meshes)
			{
				mesh->~Mesh();
				if (mesh->get_texture())
					mesh->get_texture()->~Texture();
				mesh->access_shader()->~Shader();
			}
		}

		for (auto& [id, scene] : scenes)
		{
			for (ContextualLayer*& layer : scene->contextual_layers)
			{
				for (const auto& mesh : layer->meshes)
				{
					mesh->~Mesh();
					if (mesh->get_texture())
						mesh->get_texture()->~Texture();
					mesh->access_shader()->~Shader();
				}
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
		if (layer.z_position < Camera::position.z)
			return;

		Camera::update_zoom(layer.z_position - Camera::position.z);

		for (auto& [uniform, value] : layer.uniforms)
		{
			uniform->refresh(value);
		}

		for (const auto& mesh : layer.meshes)
		{
			mesh->draw();
		}

		for (auto& [uniform, value] : layer.uniforms)
		{
			uniform->refresh();
		}
	}

	void draw_scene(unsigned int scene_parameter)
	{
		for (Layer* lay : scenes[scene_parameter]->layers)
		{
			draw(*lay);
		}
		for (ContextualLayer* cl : scenes[scene_parameter]->contextual_layers)
		{
			if (*cl->context)
				draw(*cl);
		}
	}

	void draw_current_scene()
	{
		if (scenes.find(scene_id) == scenes.end())
			return;

		draw_scene(scene_id);

		for (ContextualLayer* cl : contextual_layers)
		{
			if (*cl->context)
				draw(*cl);
		}
	}

	Vec2<float> transform;
	Renderer::Uniform<Vec2<float>> u_transform;

}