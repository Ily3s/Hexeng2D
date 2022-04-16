#include <algorithm>

#include "Renderer.hpp"
#include "../Macros.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Presets/Basic.shader"
#include "Uniform.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "../Hexeng.hpp"
#include "Presets/Presets.hpp"

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

		ToBeInit::start_init = true;
		ToBeInit([]() {});
	}

	bool ToBeInit::start_init = false;

	ToBeInit::ToBeInit(std::function<void(void)> init_object, int order)
	{
		using TBIO = std::pair<std::function<void(void)>, int>;
		static std::vector<TBIO> objects;
		objects.emplace_back(init_object, order);

		if (start_init)
		{
			std::sort(objects.begin(), objects.end(), [](const TBIO& obj1, const TBIO& obj2) {return obj1.second < obj2.second; });
			for (auto& [init_function, order] : objects)
				init_function();
		}
	}

	std::unordered_map<void*, std::function<void(void)>> ToBeDelete::objects;

	ToBeDelete::ToBeDelete(void* obj_ptr, std::function<void(void)> delete_function)
	{
		objects.insert({ obj_ptr, delete_function });
	}

	void ToBeDelete::remove(void* obj_ptr)
	{
		objects.erase(obj_ptr);
	}

	std::vector<UniformInterface*> uniform_list;

	void refresh_uniforms()
	{
		for (UniformInterface* ui : uniform_list)
			ui->refresh();
	}

	void stop()
	{
		for (auto& [ptr, delete_function] : ToBeDelete::objects)
			delete_function();

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
		if (layer.z_position < Camera::position.z && layer.position_mode == Position::RELATIVE)
			return;

		Camera::update_zoom(layer.z_position - Camera::position.z);

		for (auto& [uniform, value] : layer.uniforms)
			uniform->refresh(value);

		for (const auto& mesh : layer.meshes)
			mesh->draw();

		for (auto& [uniform, value] : layer.uniforms)
			uniform->refresh();
	}

	void draw_scene(int scene_parameter)
	{
		for (Layer* lay : scenes[scene_parameter]->layers)
			draw(*lay);

		for (ContextualLayer* cl : scenes[scene_parameter]->contextual_layers)
		{
			if (*cl->context)
				draw(*cl);
		}
	}

	std::vector<std::function<void(void)>> pending_actions;

	void draw_current_scene()
	{
		for (auto& action : pending_actions)
			action();

		pending_actions.clear();

		if (scenes.find(scene_id) == scenes.end())
			return;

		draw_scene(scene_id);

		for (Layer* l : global_layers)
			draw(*l);

		for (ContextualLayer* cl : global_contextual_layers)
		{
			if (*cl->context)
				draw(*cl);
		}
	}

	Uniform<Vec2<float>> u_transform;
	Uniform<float> u_rotation_angle;
	Uniform<Vec2<int>> u_window_size;

	ToBeInit init_uniforms
	{ []() {
		u_transform = { "u_transform", &transform, {} };
		u_rotation_angle = { "u_rotation_angle", &rotation_angle, {} };
		u_window_size = { "u_window_size", &Settings::window_size, {} };
		UniformInterface::necessary_uniforms.push_back(&u_transform);
		UniformInterface::necessary_uniforms.push_back(&u_window_size);
		UniformInterface::necessary_uniforms.push_back(&u_rotation_angle);
	} };

	Vec2<float> transform;
	float rotation_angle = 0;

}