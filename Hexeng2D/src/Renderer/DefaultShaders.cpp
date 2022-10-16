#include "DefaultShaders.hpp"
#include "DefaultShaders.glsl"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{
	Shader basic_shader;
	Shader line_shader;
	Shader font_shader;
	Shader tex_vector_shader;
	Shader batching_shader;
	Shader poly_shader;

	ToBeInit init_presets
	{ []() {
		basic_shader = Shader(basic_vs, basic_fs);
		line_shader = Shader(basic_vs, line_fs);
		font_shader = Shader(tex_vector_vs, font_fs);
		tex_vector_shader = Shader(tex_vector_vs, basic_fs);
		batching_shader = Shader(batching_vs, batching_fs);
		poly_shader = Shader(poly_vs, poly_fs);

		basic_shader.add_necessary_uniforms();
		line_shader.add_necessary_uniforms();
		font_shader.add_necessary_uniforms();
		tex_vector_shader.add_necessary_uniforms();
		batching_shader.add_necessary_uniforms();
		poly_shader.add_necessary_uniforms();
	}, 1 };

}