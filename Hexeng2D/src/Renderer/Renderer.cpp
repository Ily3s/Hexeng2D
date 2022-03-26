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

		Camera::init();
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
		if (layer.z_position < Camera::position.z)
			return;

		Vec3 cam_copy = Camera::position;
		if (layer.is_absolute)
		{
			Camera::position = { 0, 0, 0 };
			Camera::u_cam.refresh();
			Camera::zoom = 1.0f;
			Camera::u_zoom.refresh();
		}
		else
			Camera::update_zoom(layer.z_position - Camera::position.z);
		for (const auto& mesh : layer.meshes)
		{
			mesh->draw();
		}
		if (layer.is_absolute)
		{
			Camera::position = cam_copy;
			Camera::u_cam.refresh();
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
		if (scenes.find(scene_id) == scenes.end())
			return;

		for (Layer* lay : scenes[scene_id]->layers)
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