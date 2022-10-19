#include <chrono>

#include "EventManager.hpp"
#include "../Scene.hpp"
#include "../Renderer/Renderer.hpp"

#include "GLFW/glfw3.h"

namespace Hexeng::EventManager
{
	std::thread event_thread;

	Vec2<double> mouse_position{ 0, 0 };
	std::vector<Event*> global_events;

	Event::Event(std::function<bool(void)> condition, std::function<void(void)> action, Range range, uint32_t pertick)
		: condition(condition),
		action(action),
		pertick(pertick),
		range(range),
		clock(pertick - 1)
	{
		if (range == Range::GLOBAL)
			global_events.push_back(this);
	}

	EventGate::EventGate(std::function<void(void)> evt, Range range_para, unsigned int pertick_para)
	{
		action = evt;
		pertick = pertick_para;
		condition = []() {return true; };
		range = range_para;
		if (range == Range::GLOBAL)
			global_events.push_back(this);
	}

	RendererEvent::RendererEvent(std::function<bool(void)> condition_para, std::function<void(void)> action, Range range_para, unsigned int pertick_para)
		: internal_action(action)
	{
		condition = condition_para;
		range = range_para;
		pertick = pertick_para;
		action = [this]() {Renderer::pending_actions.push_back(internal_action); };
		if (range == Range::GLOBAL)
			global_events.push_back(this);
	}

	RendererEventGate::RendererEventGate(std::function<void(void)> evt, Range range_para, unsigned int pertick_para)
	{
		internal_action = evt;
		pertick = pertick_para;
		range = range_para;
		condition = []() {return true; };
		action = [this]() {Renderer::pending_actions.push_back(internal_action); };
		if (range == Range::GLOBAL)
			global_events.push_back(this);
	}

	Event::Event(Event&& other) noexcept
		: condition(other.condition),
		action(other.action),
		pertick(other.pertick),
		range(other.range),
		clock(other.clock),
		enable(other.enable)
	{
		enable_ptr = other.enable_ptr == &other.enable ? &enable : other.enable_ptr;

		if (range == Range::GLOBAL)
		{
			auto it = std::find(global_events.begin(), global_events.end(), &other);
			if (it != global_events.end())
				global_events.erase(it);
			global_events.push_back(this);
		}
	}

	Event& Event::operator=(Event&& other) noexcept
	{
		condition = other.condition;
		action = other.action;
		pertick = other.pertick;
		clock = other.clock;
		enable = other.enable;
		enable_ptr = other.enable_ptr == &other.enable ? &enable : other.enable_ptr;

		if (range == Range::GLOBAL)
		{
			auto it = std::find(global_events.begin(), global_events.end(), this);
			if (it != global_events.end())
				global_events.erase(it);
		}
		if (other.range == Range::GLOBAL)
		{
			auto it = std::find(global_events.begin(), global_events.end(), &other);
			if (it != global_events.end())
				global_events.erase(it);
			global_events.push_back(this);
		}

		range = other.range;

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
			mouse_position.y = 0.5 * Settings::window_size.y - mouse_position.y;
			mouse_position.x -= 0.5 * Settings::window_size.x;
			mouse_position /= (double)Settings::window_size.y / 1080;

			for (auto evt : global_events)
			{
				if (!*(evt->enable_ptr))
					continue;

				if (evt->clock > 0)
					evt->clock--;
				else if (evt->condition())
				{
					evt->action();
					evt->clock = evt->pertick - 1;
				}
			}

			HXG_ASSERT((scenes.find(scene_id) != scenes.end()),
				HXG_LOG_ERROR("The scene " + std::to_string(scene_id) + " doesn't exist."); goto end_loop;);

			for (auto evt : scenes[scene_id]->events)
			{
				if (!*(evt->enable_ptr))
					continue;

				if (evt->clock > 0)
					evt->clock--;
				else if (evt->condition())
				{
					evt->action();
					evt->clock = evt->pertick - 1;
				}
			}
			
		end_loop :

			current_tick++;

			auto end_point = std::chrono::high_resolution_clock::now();

			std::chrono::duration<float> duration = end_point - start_point;
			int duration_ms = static_cast<int>(duration.count() * 1000);

			if (duration_ms < 10)
				std::this_thread::sleep_for(std::chrono::milliseconds(10 - duration_ms));
		}
	}
}