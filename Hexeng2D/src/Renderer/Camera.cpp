#include "Camera.hpp"
#include <cmath>

#define PI 3.14159265359

namespace Hexeng::Renderer
{

	Vec3<int> Camera::position = { 0, 0, 0 };
	int Camera::fov = 120;
	float Camera::zoom = 1.0f;

	Uniform<Vec3<int>> Camera::u_cam;
	Uniform<float> Camera::u_zoom;

	void Camera::init()
	{
		u_cam = { "u_cam", &position, {} };
		u_zoom = { "u_zoom",&zoom, {} };
	}

	void Camera::update_zoom(int distance)
	{
		double radius = std::tan(fov / 2 * PI / 180) * distance;
		double surface = PI * std::pow(radius, 2);
		zoom = 1920*1080 * 1/surface;
		u_zoom.refresh();
	}

}