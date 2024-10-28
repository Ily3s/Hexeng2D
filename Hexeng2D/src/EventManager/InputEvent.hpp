#ifndef INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "GLFW/glfw3.h"

#include "EventManager.hpp"

namespace Hexeng::EventManager
{

	class HXG_DECLSPEC KeyEvent : public Event
	{
	public:

		/// <summary>
		/// Every pertick ticks, execute action (in the EventManager loop) if the key key_code is pressed
		/// </summary>
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>
		/// @note key_code is a glfw key code
		
		KeyEvent(int key_code, std::function<void(void)> action, Range range = Range::GLOBAL, unsigned int pertick = 1);

		KeyEvent() = default;
	};

	class HXG_DECLSPEC MouseButtonEvent : public Event
	{
	public:

		/// <summary>
		/// Every pertick ticks, execute action (in the EventManager loop) if the mouse button button_code is pressed
		/// </summary>
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>
		/// @note button_code is a glfw button code

		MouseButtonEvent(int button_code, std::function<void(void)> action, Range range = Range::GLOBAL, unsigned int pertick = 1);

		MouseButtonEvent() = default;
	};

	/// <summary>
	/// Unlike other events, ScrollEvent is a singleton and isn't related to the EventManager loop
	/// </summary>
	
	class HXG_DECLSPEC ScrollEvent
	{
	private :

		static void scroll_callback(GLFWwindow* win, double xoffset, double yoffset);
		ScrollEvent() = default;

	public :

		/// <summary>
		/// Reassign callback to set a mouse scroll callback.
		/// It takes in parameter the "amount" of the scroll.
		/// </summary>
		
		std::function<void(double)> callback = nullptr;

		static ScrollEvent* get();
	};

	/// <summary>
	/// Unlike other events, KeyPressEvent isn't related to the EventManager loop.
	/// </summary>

	class HXG_DECLSPEC KeyPressEvent
	{
	private :

		static std::vector<KeyPressEvent*> events;

		static void key_callback(GLFWwindow* win, int key, int, int action, int);

	public :

		std::function<void(void)> action;
		int key_code = GLFW_MOUSE_BUTTON_1;
		int mode = GLFW_PRESS;

		/// <summary>
		/// Executes action each time the key or button key_code is triggered in the "mode" mode
		/// </summary>
		/// <param name="key_code">A glfw key or button code</param>
		/// <param name="mode">The mode of the trigger (GLFW_PRESS, GLFW_RELEASE, ...)</param>
		
		KeyPressEvent(int key_code, std::function<void(void)> action, int mode = GLFW_PRESS);

		KeyPressEvent() = default;

		KeyPressEvent(KeyPressEvent&&) noexcept;
		KeyPressEvent& operator=(KeyPressEvent&&) noexcept;

		/// @note Can always be modified. Useless if enable_ptr != &enable.
		bool enable = true;

		/// @note Can always be modified.
		bool* enable_ptr = &enable;
	};

}

#endif