#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <unordered_map>
#include <functional>
#include <thread>

#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "../Hexeng.hpp"

namespace Hexeng::EventManager
{

	HXG_DECLSPEC extern std::thread event_thread;

	/// @note One tick of the EventManager is set to 10ms.
	/// If the EventManager is overloaded however, it can be more than 10ms.
	
	class HXG_DECLSPEC Event
	{
	public:

		std::function<bool(void)> condition = nullptr;
		std::function<void(void)> action = nullptr;
		uint32_t pertick = 1;
		uint32_t clock = 0;
		Range range = Range::GLOBAL;

		/// <summary>
		/// Every pertick ticks, action is executed (in the EventManager loop) if condition is true.
		/// </summary>
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>
		
		Event(std::function<bool(void)> condition, std::function<void(void)> action, Range range = Range::GLOBAL, uint32_t pertick = 1);

		Event() = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

		Event(Event&&) noexcept;
		Event& operator=(Event&&) noexcept;

		/// @note Can always be modified. Useless if enable_ptr != &enable.
		bool enable = true;

		/// @note Can always be modified.
		bool* enable_ptr = &enable;

	private :

		virtual void m_polymorphic();

		friend HXG_DECLSPEC void start_looping();

		static void m_loop_impl();
	};

	/// <summary>
	/// A gate to the EventManager loop
	/// </summary>
	
	class HXG_DECLSPEC EventGate : public Event
	{
	public :

		/// <summary>
		/// Every pertick ticks, evt is executed in the EventManager loop
		/// </summary>
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>
		
		EventGate(std::function<void(void)> evt, Range range = Range::GLOBAL, unsigned int pertick = 1);

		EventGate() = default;
	};

	/// <summary>
	/// An event but has access to the renderer and opengl
	/// </summary>
	/// @note The event isn't triggered until the next frame but it can be triggered multiple times within a frame.
	
	class HXG_DECLSPEC RendererEvent : public Event
	{
	public :

		std::function<void(void)> internal_action;

		RendererEvent() = default;

		/// <summary>
		/// Every pertick ticks, action is pushed back in the pending renderer actions list if condition is true.
		/// </summary>
		/// @details All the actions in the pending renderer actions list are executed the next frame.
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>
		
		RendererEvent(std::function<bool(void)> condition, std::function<void(void)> action, Range range = Range::GLOBAL, unsigned int pertick = 1);
	};

	/// <summary>
	/// RendererEventGate is to RendererEvent what EventGate is to Event
	/// </summary>
	
	class HXG_DECLSPEC RendererEventGate : public RendererEvent
	{
	public :

		/// <summary>
		/// Every pertick ticks, evt is pushed back in the pending renderer actions list.
		/// </summary>
		/// @details All the actions in the pending renderer actions list are executed the next frame.
		/// <param name="range">If range is set to local, the event is related to the scene.
		/// If it is set to global, the event is global and executed independently</param>

		RendererEventGate(std::function<void(void)> evt, Range range = Range::GLOBAL, unsigned int pertick = 1);
	};

	HXG_DECLSPEC extern Vec2<double> mouse_position;
	HXG_DECLSPEC extern std::vector<Event*> global_events;

	HXG_DECLSPEC void start_looping();
	HXG_DECLSPEC void stop_looping();

	HXG_DECLSPEC extern uint64_t current_tick;

}

#endif