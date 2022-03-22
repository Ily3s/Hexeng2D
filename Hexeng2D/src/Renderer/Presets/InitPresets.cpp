#include "InitPresets.hpp"
#include "Basic.shader"
#include "BasicQuad.hpp"
#include "../../Variables.hpp"
#include "../Uniform.hpp"

namespace Hexeng::Renderer::Presets
{

	Vec2<float> transform{ 0.0f, 0.0f };
	float zoom = 1.0f;

	Uniform<Vec2<float>> u_cam;
	Uniform<float> u_zoom;
	Uniform<Vec2<float>> u_transform;

	Shader basic_shader;
	VertexLayout basic_vertex_layout;

	void init()
	{
		basic_shader = Shader(basic_vs, basic_fs);

		u_cam = { "u_cam", &cam_position, {&basic_shader} };
		u_zoom = { "u_zoom",& zoom, {&basic_shader} };
		u_transform = { "u_transform", &transform, {&basic_shader} };

		basic_vertex_layout = VertexLayout({ VertexElement{ 2, GL_FLOAT }, VertexElement{ 2, GL_FLOAT } });

		BasicQuad::init();
	}

	void stop()
	{
		BasicQuad::stop();
	}

}