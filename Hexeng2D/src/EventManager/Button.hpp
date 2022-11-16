#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <unordered_map>

#include "InputEvent.hpp"

namespace Hexeng::EventManager
{

	/// <summary>
	/// Different types of event that can trigger a Button
	/// </summary>
	enum class ButtonEvent
	{
		HOVER,
		UNHOVER,
		CLICK,
		UNCLICK,
		KEEP_CLICKING
	};

	class HXG_DECLSPEC Button : public Event
	{
	private :

		friend class Event;

		bool m_was_hovered = false;
		bool m_was_clicking = false;
		bool m_click_outside = true;

	public :

		int mouse_button = GLFW_MOUSE_BUTTON_1;
		std::function<void(void)> hover, unhover, click, unclick, keep_clicking;
		Vec2<int> min{0, 0}, max{0, 0};

		/// <param name="min">Bottom left position of the button</param>
		/// <param name="max">Top right position of the button</param>
		/// <param name="condition">A condition that activates or deactivates the button</param>
		/// <param name="events">A map of ButtonEvent and the action it triggers</param>
		/// <param name="mouse_button">The mouse button that can trigger the button</param>
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>
		
		Button(Vec2<int> min, Vec2<int> max, std::unordered_map<ButtonEvent, std::function<void(void)>> events, int mouse_button = GLFW_MOUSE_BUTTON_1, Range range = Range::GLOBAL);

		Button() = default;

		Button(Button&&) noexcept;
		Button& operator=(Button&&) noexcept;
	};

}

#endif