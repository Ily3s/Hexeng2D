#include <cmath>

#include "Camera.hpp"
#include "../Hexeng.hpp"
#include "Renderer.hpp"

#define PI 3.14159265359

namespace Hexeng::Renderer
{

	Vec3<int> Camera::position = { 0, 0, 0 };
	Vec2<int> Camera::s_xy_position = {0, 0};
	const Vec2<int>* Camera::xy_position = &Camera::s_xy_position;
	Vec2<float> Camera::s_shader_pos = { 0.0f, 0.0f };
	int Camera::fov = 120;
	float Camera::s_zoom = 1.0f;

	Uniform<Vec2<float>> Camera::u_cam;
	Uniform<float> Camera::u_zoom;

	ToBeInit Camera::s_init_cam
	{ std::function<void(void)>{ []() {
		Camera::u_cam = { {	{UniformArgType::NAME, "u_cam"},
							{UniformArgType::CONTROLLLER, &Camera::s_shader_pos} } };
		Camera::u_zoom = { { {UniformArgType::NAME, "u_zoom"},
							{UniformArgType::CONTROLLLER, &Camera::s_zoom},
							{UniformArgType::FUSION_MODE, (void*)UniformFusionMode::MULTIPLY} } };
		UniformInterface::necessary_uniforms.push_back(&Camera::u_cam);
		UniformInterface::necessary_uniforms.push_back(&Camera::u_zoom);
	} } };

	void Camera::s_update_zoom(int distance)
	{
		s_xy_position = { position.x, position.y };
		double radius = std::tan(fov / 2 * PI / 180) * distance;
		double surface = PI * std::pow(radius, 2);
		s_zoom = 1920*1080 * 1/surface;
		u_zoom.refresh();
	}

	void Camera::refresh_pos()
	{
		s_shader_pos = toCoord({position.x, position.y});
	}

}