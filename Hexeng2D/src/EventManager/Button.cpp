#include "Button.hpp"
#include "GLFW/glfw3.h"

namespace Hexeng::EventManager
{

	Button::Button(Vec2<int> min_para, Vec2<int> max_para, std::unordered_map<ButtonEvent, std::function<void(void)>> events_para, int mouse_btn, Range range)
		: min(min_para),
		max(max_para),
		mouse_button(mouse_btn)
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
		
		Event::operator=(Event([]() {return true; },
			[this]() {
				if (mouse_position.x >= min.x && mouse_position.x <= max.x && mouse_position.y >= min.y && mouse_position.y <= max.y)
				{
					if (hover && !m_was_hovered)
						hover();
					m_was_hovered = true;
					if (glfwGetMouseButton(window, mouse_button))
					{
						if (!m_was_clicking && !m_click_outside)
						{
							if (click)
								click();
							m_was_clicking = true;
						}
						else if (keep_clicking && !m_click_outside)
							keep_clicking();
					}
					else if (m_was_clicking)
					{
						if (unclick)
							unclick();
						m_was_clicking = false;
					}
					else
						m_click_outside = false;
				}
				else if (m_was_hovered)
				{
					if (unhover)
						unhover();
					m_was_hovered = false;
					
					if (m_was_clicking && unclick)
						unclick();
				}
				else
				{
					if (glfwGetMouseButton(window, mouse_button))
					{
						if (!m_was_clicking)
							m_click_outside = true;
					}
					else
					{
						m_click_outside = false;
						m_was_clicking = false;
					}
				}
			},
			range, 1));
	}

	Button::Button(Button&& other) noexcept
		: Event(std::move(other)),
		mouse_button(other.mouse_button),
		min(other.min),
		max(other.max),
		hover(other.hover),
		unhover(other.unhover),
		click(other.click),
		unclick(other.unclick),
		keep_clicking(other.keep_clicking)
	{
		action = [this]() {
			if (mouse_position.x >= min.x && mouse_position.x <= max.x && mouse_position.y >= min.y && mouse_position.y <= max.y)
			{
				if (hover && !m_was_hovered)
					hover();
				m_was_hovered = true;
				if (glfwGetMouseButton(window, mouse_button))
				{
					if (!m_was_clicking && !m_click_outside)
					{
						if (click)
							click();
						m_was_clicking = true;
					}
					else if (keep_clicking && !m_click_outside)
						keep_clicking();
				}
				else if (m_was_clicking)
				{
					if (unclick)
						unclick();
					m_was_clicking = false;
				}
				else
					m_click_outside = false;
			}
			else if (m_was_hovered)
			{
				if (unhover)
					unhover();
				m_was_hovered = false;

				if (m_was_clicking && unclick)
					unclick();
			}
			else
			{
				if (glfwGetMouseButton(window, mouse_button))
				{
					if (!m_was_clicking)
						m_click_outside = true;
				}
				else
				{
					m_click_outside = false;
					m_was_clicking = false;
				}
			}
		};
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

		action = [this]() {
			if (mouse_position.x >= min.x && mouse_position.x <= max.x && mouse_position.y >= min.y && mouse_position.y <= max.y)
			{
				if (hover && !m_was_hovered)
					hover();
				m_was_hovered = true;
				if (glfwGetMouseButton(window, mouse_button))
				{
					if (!m_was_clicking && !m_click_outside)
					{
						if (click)
							click();
						m_was_clicking = true;
					}
					else if (keep_clicking && !m_click_outside)
						keep_clicking();
				}
				else if (m_was_clicking)
				{
					if (unclick)
						unclick();
					m_was_clicking = false;
				}
				else
					m_click_outside = false;
			}
			else if (m_was_hovered)
			{
				if (unhover)
					unhover();
				m_was_hovered = false;

				if (m_was_clicking && unclick)
					unclick();
			}
			else
			{
				if (glfwGetMouseButton(window, mouse_button))
				{
					if (!m_was_clicking)
						m_click_outside = true;
				}
				else
				{
					m_click_outside = false;
					m_was_clicking = false;
				}
			}
		};

		return *this;
	}

}