#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "Uniform.hpp"

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Camera
	{
	private :

		Camera() = default;

		static Vec2<float> shader_pos;

	public :

		static Vec3<int> position;
		static int fov;
		static float zoom;

		static Uniform<Vec2<float>> u_cam;
		static Uniform<float> u_zoom;

		static void init();
		static void update_zoom(int distance);
		static void refresh_pos();
	};

}

#endif