#include "InitPresets.hpp"
#include "Basic.shader"
#include "BasicQuad.hpp"
#include "../Uniform.hpp"
#include "../Camera.hpp"
#include "../Renderer.hpp"

namespace Hexeng::Renderer::Presets
{

	Shader basic_shader;
	Shader line_shader;
	Uniform<Color3> u_color;
	Color3 color{ 0.0f, 0.0f, 0.0f };
	VertexLayout basic_vertex_layout;

	void init()
	{
		basic_shader = Shader(basic_vs, basic_fs);
		line_shader = Shader(basic_vs, line_fs);

		Camera::u_cam.add_shaders({ &basic_shader, &line_shader });
		Camera::u_zoom.add_shaders({ &basic_shader, &line_shader });
		u_transform.add_shaders({ &basic_shader, &line_shader });
		u_color = { "u_color", &color, {&line_shader} };

		basic_vertex_layout = VertexLayout({ VertexElement{ 2, GL_FLOAT }, VertexElement{ 2, GL_FLOAT } });

		BasicQuad::init();
	}

	void stop()
	{
		BasicQuad::stop();

		basic_shader.~Shader();
		line_shader.~Shader();
	}

}