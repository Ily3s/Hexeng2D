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

	public :

		static Vec3<int> position;
		static int fov;
		static float zoom;

		static Uniform<Vec3<int>> u_cam;
		static Uniform<float> u_zoom;

		static void init();
		static void update_zoom(int distance);
	};

}

#endif