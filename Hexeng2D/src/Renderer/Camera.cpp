#include <cmath>

#include "Camera.hpp"
#include "../Hexeng.hpp"
#include "Renderer.hpp"

#define PI 3.14159265359

namespace Hexeng::Renderer
{

	Vec3<int> Camera::position = { 0, 0, 0 };
	Vec2<int> Camera::m_xy_position = {0, 0};
	const Vec2<int>* Camera::xy_position = &Camera::m_xy_position;
	Vec2<float> Camera::shader_pos = { 0.0f, 0.0f };
	int Camera::fov = 120;
	float Camera::zoom = 1.0f;

	Uniform<Vec2<float>> Camera::u_cam;
	Uniform<float> Camera::u_zoom;

	ToBeInit init_cam
	{ []() {
		Camera::u_cam = { {	{UniformArgType::NAME, "u_cam"},
							{UniformArgType::CONTROLLLER, &Camera::shader_pos} } };
		Camera::u_zoom = {{ {UniformArgType::NAME, "u_zoom"},
							{UniformArgType::CONTROLLLER, &Camera::zoom} } };
		UniformInterface::necessary_uniforms.push_back(&Camera::u_cam);
		UniformInterface::necessary_uniforms.push_back(&Camera::u_zoom);
	} };

	void Camera::update_zoom(int distance)
	{
		m_xy_position = { position.x, position.y };
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