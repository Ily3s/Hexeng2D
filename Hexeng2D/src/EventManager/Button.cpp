#include "Button.hpp"
#include "GLFW/glfw3.h"

namespace Hexeng::EventManager
{

	Button::Button(Vec2<int> min_para, Vec2<int> max_para, std::function<bool(void)> condition_para, std::unordered_map<ButtonEvent, std::function<void(void)>> events_para, int mouse_btn, Range range)
		: min(min_para),
		max(max_para),
		mouse_button(mouse_btn),
		condition(condition_para)
	{
		for (auto& [evt, action] : events_para)
		{
			switch (evt)
			{
			case ButtonEvent::HOVER :
				hover = action;
				break;

			case ButtonEvent::UNHOVER :
				unhover = action;
				break;

			case ButtonEvent::CLICK :
				click = action;
				break;

			case ButtonEvent::UNCLICK :
				unclick = action;
				break;

			case ButtonEvent::KEEP_CLICKING :
				keep_clicking = action;
				break;

			default:
				break;
			}
		}
		
		Event::operator=( Event( condition,
			[this]() {
				static Button* owner = this;
				owner = m_owner;

				if (mouse_position.x >= owner->min.x && mouse_position.x <= owner->max.x && mouse_position.y >= owner->min.y && mouse_position.y <= owner->max.y)
				{
					if (owner->hover)
						owner->hover();
					owner->m_was_hovered = true;
					if (glfwGetMouseButton(window, owner->mouse_button))
					{
						if (!owner->m_was_clicking && !owner->m_click_outside)
						{
							if (owner->click)
								owner->click();
							owner->m_was_clicking = true;
						}
						else if (owner->keep_clicking)
							owner->keep_clicking();
					}
					else if (owner->m_was_clicking)
					{
						if (owner->unclick)
							owner->unclick();
						owner->m_was_clicking = false;
					}
					else
						owner->m_click_outside = false;
				}
				else if (owner->m_was_hovered)
				{
					if (owner->unhover)
						owner->unhover();
					owner->m_was_hovered = false;
					
					if (owner->m_was_clicking)
					{
						if (owner->unclick)
							owner->unclick();
						owner->m_was_clicking = false;
					}
				}
				else
				{
					if (glfwGetMouseButton(window, owner->mouse_button))
						owner->m_click_outside = true;
					else
						owner->m_click_outside = false;
				}
			},
			range, 1));
	}

	Button::Button(Button&& other) noexcept
		: Event(std::move(other)),
		mouse_button(other.mouse_button),
		min(other.min),
		max(other.max),
		m_owner(this),
		condition(other.condition),
		hover(other.hover),
		unhover(other.unhover),
		click(other.click),
		unclick(other.unclick),
		keep_clicking(other.keep_clicking)
	{
		other.m_owner = this;
	}

	Button& Button::operator=(Button&& other) noexcept
	{
		Event::operator=(std::move(other));
		mouse_button = other.mouse_button;
		condition = other.condition;
		hover = other.hover;
		unhover = other.unhover;
		click = other.click;
		unclick = other.unclick;
		keep_clicking = other.keep_clicking;
		min = other.min;
		max = other.max;
		other.m_owner = this;

		return *this;
	}

}