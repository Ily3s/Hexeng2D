#include "GLFW/glfw3.h"

#include "InputEvent.hpp"

#include <algorithm>

namespace Hexeng::EventManager
{

	KeyEvent::KeyEvent(int key_code, std::function<void(void)> action_arg, Range range_para, unsigned int pertick_arg)
	{
		pertick = pertick_arg;
		action = action_arg;
		condition = [key_code]() {return glfwGetKey(window, key_code); };
		range = range_para;
		if (range == Range::GLOBAL)
			global_events.push_back(this);
	}

	MouseButtonEvent::MouseButtonEvent(int button_code, std::function<void(void)> action_arg, Range range_para, unsigned int pertick_arg)
	{
		pertick = pertick_arg;
		action = action_arg;
		condition = [button_code]() {return glfwGetMouseButton(window, button_code); };
		range = range_para;
		if (range == Range::GLOBAL)
			global_events.push_back(this);
	}

	ScrollEvent* ScrollEvent::get()
	{
		static ScrollEvent* instance = nullptr;
		if (!instance)
		{
			static ScrollEvent singleton{};
			instance = &singleton;
			HXG_GLFW(glfwSetScrollCallback(window, scroll_callback));
		}
		return instance;
	}

	void ScrollEvent::scroll_callback(GLFWwindow* win, double xoffset, double yoffset)
	{
		std::lock_guard<std::mutex> safe_render(safe_render_mutex);
		get()->callback(yoffset);
	}

	std::vector<KeyPressEvent*> KeyPressEvent::events;

	KeyPressEvent::KeyPressEvent(int key_code, std::function<void(void)> action, int mode)
		: key_code(key_code),
		action(action),
		mode(mode)
	{
		events.push_back(this);

		static bool is_init = false;

		if (!is_init)
		{
			HXG_GLFW(glfwSetKeyCallback(window, key_callback));
			is_init = true;
		}
	}

	void KeyPressEvent::key_callback(GLFWwindow* win, int key, int, int action, int)
	{
		for (KeyPressEvent* evt : events)
		{
			if (key == evt->key_code && action == evt->mode && *(evt->enable_ptr))
			{
				std::lock_guard<std::mutex> safe_render(safe_render_mutex);

				evt->action();
			}
		}
	}

	KeyPressEvent::KeyPressEvent(KeyPressEvent&& other) noexcept
		: action(other.action), key_code(other.key_code), mode(other.mode), enable(other.enable)
	{
		enable_ptr = other.enable_ptr == &other.enable ? &enable : other.enable_ptr;
		auto it = std::find(events.begin(), events.end(), &other);
		if (it != events.end())
			*it = this;
	}

	KeyPressEvent& KeyPressEvent::operator=(KeyPressEvent&& other) noexcept
	{
		action = other.action;
		key_code = other.key_code;
		mode = other.mode;
		enable = other.enable;
		enable_ptr = other.enable_ptr == &other.enable ? &enable : other.enable_ptr;

		auto it = std::find(events.begin(), events.end(), this);
		if (it != events.end())
			events.erase(it);

		it = std::find(events.begin(), events.end(), &other);
		if (it != events.end())
			*it = this;

		return *this;
	}

}
