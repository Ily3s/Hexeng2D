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
	Uniform<Color3> u_color;

	ToBeInit init_presets
	{ []() {
		basic_shader = Shader(basic_vs, basic_fs);
		line_shader = Shader(basic_vs, line_fs);
		font_shader = Shader(tex_vector_vs, font_fs);
		tex_vector_shader = Shader(tex_vector_vs, basic_fs);
		batching_shader = Shader(batching_vs, basic_fs);

		basic_shader.add_necessary_uniforms();
		line_shader.add_necessary_uniforms();
		font_shader.add_necessary_uniforms();
		tex_vector_shader.add_necessary_uniforms();
		batching_shader.add_necessary_uniforms();

		u_color = {{{UniformArgType::NAME, "u_color"}}};
		u_color.add_shaders({ &line_shader, &font_shader, &batching_shader });
	}, 1 };

}