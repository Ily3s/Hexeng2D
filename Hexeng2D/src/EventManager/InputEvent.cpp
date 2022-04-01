#include "GLFW/glfw3.h"

#include "../Hexeng.hpp"
#include "InputEvent.hpp"

namespace Hexeng::EventManager
{

	KeyEvent::KeyEvent(int key_code, std::function<void(void)> action_arg, unsigned int pertick_arg)
	{
		pertick = pertick_arg;
		action = action_arg;
		condition = [key_code]() {return glfwGetKey(window, key_code); };
		events.push_back({ this, pertick });
	}

	MouseButtonEvent::MouseButtonEvent(int button_code, std::function<void(void)> action_arg, unsigned int pertick_arg)
	{
		pertick = pertick_arg;
		action = action_arg;
		condition = [button_code]() {return glfwGetMouseButton(window, button_code); };
		events.push_back({ this, pertick });
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
		get()->callback(yoffset);
	}

}