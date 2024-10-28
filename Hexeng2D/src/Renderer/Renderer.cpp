#include <algorithm>

#include "Renderer.hpp"
#include "../Macros.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Uniform.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "../Hexeng.hpp"
#include "Animation.hpp"

namespace Hexeng::Renderer
{

	std::thread::id thread_id;

	void init()
	{
		thread_id = std::this_thread::get_id();

		HXG_GLFW(if (!glfwInit()) { ::exit(-1); });

		HXG_GLFW(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4));
		HXG_GLFW(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6));
		HXG_GLFW(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		Vec2<int> screen_size{ mode->width, mode->height };

		if (Settings::fullscreen)
		{
			Settings::window_size = screen_size;
			HXG_GLFW(window = glfwCreateWindow(screen_size.x, screen_size.y, Settings::window_name.c_str(), glfwGetPrimaryMonitor(), NULL));
		}
		else
		{
			HXG_GLFW(window = glfwCreateWindow(Settings::window_size.x, Settings::window_size.y, Settings::window_name.c_str(), 0, NULL));
		}

        HXG_GLFW(glfwSetWindowSizeCallback(window, _resize_callback));

		HXG_GLFW(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));

		if (!window)
		{
			glfwTerminate();
			::exit(-1);
		}

		HXG_GLFW(glfwMakeContextCurrent(window));

		HXG_GLFW(glfwSwapInterval(Settings::enable_vsync));

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			::exit(-1);

		std::cout << glGetString(GL_VERSION) << '\n' << std::endl;

		HXG_GL(glEnable(GL_BLEND));
		HXG_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		HXG_GL(glBlendEquation(GL_FUNC_ADD));

		HXG_GL(glLineWidth(2.0f));

		ToBeInit::start_init = true;
		ToBeInit([]() {});

		pending_actions.push_back([]()
			{
				for (auto& [id, scene] : scenes)
				{
					scene->unload();
					scene->layers.reserve(scene->layers.size() + global_layers.size());
					for (Layer* gl : global_layers)
						scene->layers.push_back(gl);
					std::sort(scene->layers.begin(), scene->layers.end(), [](Layer* layer1, Layer* layer2) {
						return layer1->z_position > layer2->z_position; });
				}
				scenes[scene_id]->load();
			});
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

	void draw_scene(int scene_parameter)
	{
		for (Layer* lay : scenes[scene_parameter]->layers)
			lay->draw();
	}

	std::vector<std::function<void(void)>> pending_actions;

	void draw_current_scene()
	{
		HXG_ASSERT((scenes.find(scene_id) != scenes.end()),
			HXG_LOG_ERROR("The scene " + std::to_string(scene_id) + " doesn't exist."); return;);

		for (int i = 0; i < pending_actions.size(); i++)
			pending_actions[i]();

		pending_actions.clear();

		Animation::s_update_animations();

		draw_scene(scene_id);
	}

	Uniform<Vec2<float>> u_transform;
	Uniform<float> u_rotation_angle;
	Uniform<Vec2<int>> u_window_size;
	Uniform<float> u_scale;
	Uniform<Color4> u_color_filter;
	Uniform<Color4> u_color;

	ToBeInit init_uniforms
	{ []() {
		u_transform = {{{UniformArgType::NAME, "u_transform"}}};
		u_rotation_angle = { {{UniformArgType::NAME, "u_rotation_angle"}} };
		u_window_size = { {{UniformArgType::NAME, "u_window_size"},{UniformArgType::CONTROLLLER, &Settings::window_size } } };
		u_scale = { { {UniformArgType::NAME, "u_scale"}, {UniformArgType::FUSION_MODE, (void*)UniformFusionMode::MULTIPLY} } };
		u_color_filter = { {{UniformArgType::NAME, "u_color_filter"}, {UniformArgType::FUSION_MODE, (void*)UniformFusionMode::MULTIPLY}} };
		u_color = { {{UniformArgType::NAME, "u_color"}} };
		UniformInterface::necessary_uniforms.push_back(&u_transform);
		UniformInterface::necessary_uniforms.push_back(&u_window_size);
		UniformInterface::necessary_uniforms.push_back(&u_rotation_angle);
		UniformInterface::necessary_uniforms.push_back(&u_scale);
		UniformInterface::necessary_uniforms.push_back(&u_color_filter);
		UniformInterface::necessary_uniforms.push_back(&u_color);
	} };

}
