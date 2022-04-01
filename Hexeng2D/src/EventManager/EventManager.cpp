#include <chrono>

#include "GLFW/glfw3.h"

#include "../Hexeng.hpp"
#include "EventManager.hpp"
#include "InputEvent.hpp"

namespace Hexeng::EventManager
{
	std::thread event_thread;

	Vec2<double> mouse_position{ 0, 0 };
	std::vector<std::pair<Event*, unsigned int>> events{};

	Event::Event(std::function<bool(void)> condition, std::function<void(void)> action, unsigned int pertick)
		: condition(condition), action(action), pertick(pertick)
	{
		events.push_back({ this, pertick });
	}

	std::vector<std::pair<Event*, unsigned int>>::iterator find(std::vector<std::pair<Event*, unsigned int>>& vec, Event* searching)
	{
		for (std::vector<std::pair<Event*, unsigned int>>::iterator it = vec.begin(); it < vec.end(); it++)
		{
			if (it->first == searching)
				return it;
		}
		return vec.end();
	}

	Event::Event(Event&& other) noexcept
		: condition(other.condition), action(other.action), pertick(other.pertick)
	{
		auto it = find(events, &other);
		if (it != events.end())
			events.erase(it);
		events.push_back({ this, pertick });
	}

	Event& Event::operator=(Event&& other) noexcept
	{
		condition = other.condition;
		action = other.action;
		pertick = other.pertick;

		auto it = find(events, &other);
		if (it != events.end())
			events.erase(it);
		it = find(events, this);
		if (it != events.end())
			events.erase(it);
		events.push_back({ this, pertick });

		return *this;
	}

	void loop_impl();
	bool allowed_to_loop = true;

	void start_looping()
	{
		event_thread = std::thread(loop_impl);
	}

	void stop_looping()
	{
		allowed_to_loop = false;
		event_thread.join();
	}

	uint64_t current_tick = 0;

	void loop_impl()
	{
		while (allowed_to_loop)
		{
			auto start_point = std::chrono::high_resolution_clock::now();

			HXG_GLFW(glfwGetCursorPos(window, &mouse_position.x, &mouse_position.y));

			for (auto& [evt, tick] : events)
			{
				if (tick > 1)
					tick--;
				else if (evt->condition())
				{
					evt->action();
					tick = evt->pertick;
				}
			}

			current_tick++;

			auto end_point = std::chrono::high_resolution_clock::now();

			std::chrono::duration<float> duration = end_point - start_point;
			int duration_ms = static_cast<int>(duration.count() * 1000);

			if (duration_ms < 10)
				std::this_thread::sleep_for(std::chrono::milliseconds(10 - duration_ms));
		}
	}
}