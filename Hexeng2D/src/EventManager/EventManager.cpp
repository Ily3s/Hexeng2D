#include <chrono>

#include "GLFW/glfw3.h"

#include "../Hexeng.hpp"
#include "EventManager.hpp"

namespace Hexeng::EventManager
{

	std::thread event_thread;

	Vec2<double> mouse_position{ 0, 0 };
	std::unordered_map<Event*, unsigned int> events{};

	Event::Event(std::function<bool(void)> condition, std::function<void(void)> action, unsigned int pertick)
		: condition(condition), action(action), pertick(pertick)
	{
		events.insert({ this, pertick });
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

	void loop_impl()
	{
		while (allowed_to_loop)
		{
			auto start_point = std::chrono::high_resolution_clock::now();

			HXG_GLFW(glfwGetCursorPos(window, &mouse_position.x, &mouse_position.y));

			for (auto& [event, tick] : events)
			{
				if (tick > 1)
					tick--;
				else if (event->condition())
				{
					event->action();
					tick = event->pertick;
				}
			}

			auto end_point = std::chrono::high_resolution_clock::now();

			std::chrono::duration<float> duration = end_point - start_point;
			int duration_ms = static_cast<int>(duration.count() * 1000);

			if (duration_ms < 10)
				std::this_thread::sleep_for(std::chrono::milliseconds(10 - duration_ms));
		}
	}
}