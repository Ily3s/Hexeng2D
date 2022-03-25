#include "InitPresets.hpp"
#include "Basic.shader"
#include "BasicQuad.hpp"
#include "../Uniform.hpp"
#include "../Camera.hpp"

namespace Hexeng::Renderer::Presets
{

	Vec2<float> transform{ 0.0f, 0.0f };
	Uniform<Vec2<float>> u_transform;

	Shader basic_shader;
	VertexLayout basic_vertex_layout;

	void init()
	{
		basic_shader = Shader(basic_vs, basic_fs);

		Camera::u_cam.add_shaders({ &basic_shader });
		Camera::u_zoom.add_shaders({ &basic_shader });
		u_transform = { "u_transform", &transform, {&basic_shader} };

		basic_vertex_layout = VertexLayout({ VertexElement{ 2, GL_FLOAT }, VertexElement{ 2, GL_FLOAT } });

		BasicQuad::init();
	}

	void stop()
	{
		BasicQuad::stop();
	}

}