#include "Presets.hpp"
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

	ToBeInit init_presets
	{ []() {
		basic_shader = Shader(basic_vs, basic_fs);
		line_shader = Shader(basic_vs, line_fs);

		basic_shader.add_necessary_uniforms();
		line_shader.add_necessary_uniforms();

		u_color = { "u_color", &color, {&line_shader} };

		basic_vertex_layout = VertexLayout({ VertexElement{ 2, GL_FLOAT }, VertexElement{ 2, GL_FLOAT } });

		BasicQuad::init();
	}, 1 };

	void stop()
	{
		BasicQuad::stop();

		basic_shader.~Shader();
		line_shader.~Shader();
	}

}