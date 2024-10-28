#include "DefaultShaders.hpp"
#include "DefaultShaders.glsl"
#include "Renderer.hpp"
#include "BatchRenderer.hpp"

namespace Hexeng::Renderer
{
	Shader basic_shader;
	Shader line_shader;
	Shader font_shader;
	Shader tex_vector_shader;
	BatchingShader batching_shader_250;
	Shader poly_shader;

	ToBeInit init_presets
	{ []() {
		basic_shader = Shader({ {ShaderType::VERTEX_SHADER, basic_vs}, {ShaderType::FRAGMENT_SHADER, basic_fs} });
		line_shader = Shader({ {ShaderType::VERTEX_SHADER,basic_vs} , {ShaderType::FRAGMENT_SHADER, line_fs} });
		font_shader = Shader({ {ShaderType::VERTEX_SHADER, tex_vector_vs}, {ShaderType::FRAGMENT_SHADER, font_fs} });
		tex_vector_shader = Shader({ {ShaderType::VERTEX_SHADER, tex_vector_vs}, {ShaderType::FRAGMENT_SHADER, basic_fs} });
		poly_shader = Shader({ {ShaderType::VERTEX_SHADER, poly_vs}, {ShaderType::FRAGMENT_SHADER, poly_fs} });

		basic_shader.add_necessary_uniforms();
		line_shader.add_necessary_uniforms();
		font_shader.add_necessary_uniforms();
		tex_vector_shader.add_necessary_uniforms();
		poly_shader.add_necessary_uniforms();

        batching_shader_250 = BatchingShader(250);
	}, 1 };

}
