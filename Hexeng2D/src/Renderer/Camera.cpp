#include <cmath>

#include "Camera.hpp"
#include "../Functions.hpp"

#define PI 3.14159265359

namespace Hexeng::Renderer
{

	Vec3<int> Camera::position = { 0, 0, 0 };
	Vec2<float> Camera::shader_pos = { 0.0f, 0.0f };
	int Camera::fov = 120;
	float Camera::zoom = 1.0f;

	Uniform<Vec2<float>> Camera::u_cam;
	Uniform<float> Camera::u_zoom;

	void Camera::init()
	{
		u_cam = { "u_cam", &shader_pos, {} };
		u_zoom = { "u_zoom",&zoom, {} };
		UniformInterface::necessary_uniforms.push_back(&u_cam);
		UniformInterface::necessary_uniforms.push_back(&u_zoom);
	}

	void Camera::update_zoom(int distance)
	{
		double radius = std::tan(fov / 2 * PI / 180) * distance;
		double surface = PI * std::pow(radius, 2);
		zoom = 1920*1080 * 1/surface;
		u_zoom.refresh();
	}

	void Camera::refresh_pos()
	{
		shader_pos = toCoord({position.x, position.y});
	}

}