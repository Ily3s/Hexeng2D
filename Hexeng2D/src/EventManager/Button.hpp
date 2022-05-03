#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <unordered_map>

#include "InputEvent.hpp"

namespace Hexeng::EventManager
{

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

		bool m_was_hovered = false;
		bool m_was_clicking = false;
		bool m_click_outside = false;

		Button* m_owner = this;

	public :

		int mouse_button = GLFW_MOUSE_BUTTON_1;
		std::function<bool(void)> condition;
		std::function<void(void)> hover, unhover, click, unclick, keep_clicking;
		Vec2<int> min{0, 0}, max{0, 0};

		Button(Vec2<int> min, Vec2<int> max, std::function<bool(void)> condition, std::unordered_map<ButtonEvent, std::function<void(void)>> events, int mouse_button = GLFW_MOUSE_BUTTON_1, Range range = Range::GLOBAL);

		Button() = default;

		Button(Button&&) noexcept;
		Button& operator=(Button&&) noexcept;
	};

}

#endif