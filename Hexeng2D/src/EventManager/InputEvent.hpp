#ifndef INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "EventManager.hpp"

namespace Hexeng::EventManager
{

	class HXG_DECLSPEC KeyEvent : public Event
	{
	public:

		KeyEvent(int key_code, std::function<void(void)> action, Range range = Range::GLOBAL, unsigned int pertick = 1);
	};

	class HXG_DECLSPEC MouseButtonEvent : public Event
	{
	public:

		MouseButtonEvent(int button_code, std::function<void(void)> action, Range range = Range::GLOBAL, unsigned int pertick = 1);
	};

	class HXG_DECLSPEC ScrollEvent
	{
	private :

		static void scroll_callback(GLFWwindow* win, double xoffset, double yoffset);
		ScrollEvent() = default;

	public :

		std::function<void(double)> callback = nullptr;
		static ScrollEvent* get();
	};

	class HXG_DECLSPEC KeyPressEvent
	{
	private :

		static std::vector<KeyPressEvent*> events;

		static void key_callback(GLFWwindow* win, int key, int, int action, int);

	public :

		std::function<void(void)> action;
		int key_code;
		int mode;

		KeyPressEvent(int key_code, std::function<void(void)> action, int mode = GLFW_PRESS);
	};

}

#endif