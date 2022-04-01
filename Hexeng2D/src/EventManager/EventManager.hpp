#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <unordered_map>
#include <functional>
#include <thread>

#include "../Macros.hpp"
#include "../Vectors.hpp"

namespace Hexeng::EventManager
{

	HXG_DECLSPEC extern std::thread event_thread;

	class HXG_DECLSPEC Event
	{
	public:

		std::function<bool(void)> condition = nullptr;
		std::function<void(void)> action = nullptr;
		unsigned int pertick = 1;

		Event(std::function<bool(void)> condition, std::function<void(void)> action, unsigned int pertick = 1);

		Event() = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

		Event(Event&&) noexcept;
		Event& operator=(Event&&) noexcept;

	};

	HXG_DECLSPEC extern Vec2<double> mouse_position;
	HXG_DECLSPEC extern std::vector<std::pair<Event*, unsigned int>> events;

	HXG_DECLSPEC void start_looping();
	HXG_DECLSPEC void stop_looping();

	HXG_DECLSPEC extern uint64_t current_tick;

}

#endif