#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "Uniform.hpp"

namespace Hexeng::Renderer
{

	class ToBeInit;

	class HXG_DECLSPEC Camera
	{
	private :

		Camera() = default;

		static Vec2<int> s_xy_position;
		static Vec2<float> s_shader_pos;

		static ToBeInit s_init_cam;

		static float s_zoom;

		friend class Layer;
		friend class ContextualLayer;

		static Uniform<Vec2<float>> u_cam;

		static Uniform<float> u_zoom;

		static void s_update_zoom(int distance);

	public :

		/// @brief Can be updated at any moment.
		/// @note After updating this value, you may want to call refresh_pos().
		static Vec3<int> position;

		/// @brief Can be updated at any moment.
		static int fov;

		/// @brief Access to the x and y position of the cam.
		static const Vec2<int>* xy_position;

        // todo : retirer cette merde, le faire toute les frames
		/// @brief After updating the position, you may want to call this function. 
		static void refresh_pos();
	};

}

#endif
