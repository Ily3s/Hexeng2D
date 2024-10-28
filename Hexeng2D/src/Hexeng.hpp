#ifndef HEXENG_HPP
#define HEXENG_HPP

#include <functional>
#include <string>
#include <mutex>

#include "Macros.hpp"
#include "Vectors.hpp"

struct GLFWwindow;

namespace Hexeng
{

	HXG_DECLSPEC extern GLFWwindow* window;

	/**
	* @brief Starts the game loop.
	* @param[in] pre A function to be executed before the render of each frame.
	* @param[in] post A function to be executed after the render of each frame.
	* @note The thread in wich this function has been called is blocked until the application is closed.
	*/
	HXG_DECLSPEC void game_loop(std::function<void()> pre = nullptr, std::function<void()> post = nullptr);

	namespace Settings
	{
		HXG_DECLSPEC extern bool enable_vsync;
		HXG_DECLSPEC extern std::string window_name;
		HXG_DECLSPEC extern bool fullscreen;
		HXG_DECLSPEC extern Vec2<int> window_size;
		HXG_DECLSPEC float get_ratio();

		HXG_DECLSPEC extern float master_volume;
		HXG_DECLSPEC extern float sound_volume;
		HXG_DECLSPEC extern float music_volume;
	}

	/// <summary>
	/// Converts x to the OpenGL default coordinate system
	/// </summary>
	/// <param name="x">An x coordinate in the Hexeng2D coordinate system</param>
	/// <returns>An x coordinate in the OpenGL coordinate system</returns>
	/// @note This function is meant to be use internally by the engine.
	inline float toX(float x) { return x / (static_cast<float>(1080) * Settings::get_ratio() / 2); }

	/// <summary>
	/// Converts y to the OpenGL default coordinate system
	/// </summary>
	/// <param name="y">A y coordinate in the Hexeng2D coordinate system</param>
	/// <returns>A y coordinate in the OpenGL coordinate system</returns>
	/// @note This function is meant to be used internally by the engine.
	inline float toY(float y) { return y / (1080 / 2); }

	/// <summary>
	/// Converts vec to the OpenGL coordinate system
	/// </summary>
	/// <param name="vec">A vector in the Hexeng2D coordinate system</param>
	/// <returns>A vector in the OpenGL coordinate system</returns>
	/// @note This function is meant to be used internally by the engine.
	inline Vec2<float> toCoord(Vec2<int> vec) { return { toX(vec.x), toY(vec.y) }; }

    void _resize_callback(GLFWwindow* window, int x, int y);

    void resize_window(int x, int y);

	enum class HXG_DECLSPEC Range
	{
		LOCAL,
		GLOBAL
	};

	HXG_DECLSPEC extern float frame_time;

	/// @brief Set to true to exit next frame.
	HXG_DECLSPEC extern bool exit;

	HXG_DECLSPEC extern std::mutex safe_render_mutex;

}

#endif
