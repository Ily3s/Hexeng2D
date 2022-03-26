#ifndef INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "EventManager.hpp"

namespace Hexeng::EventManager
{

	class HXG_DECLSPEC KeyEvent : public Event
	{
	public:

		KeyEvent(int key_code, std::function<void(void)> action, unsigned int pertick = 1);
	};

	class HXG_DECLSPEC MouseButtonEvent : public Event
	{
	public:

		MouseButtonEvent(int button_code, std::function<void(void)> action, unsigned int pertick = 1);
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

}

#endif