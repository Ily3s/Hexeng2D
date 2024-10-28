#ifndef DEFAULT_SHADERS_HPP
#define DEFAULT_SHADERS_HPP

#include <string>

#include "../Macros.hpp"
#include "Shader.hpp"
#include "Uniform.hpp"
#include "../Color.hpp"

namespace Hexeng::Renderer
{
    class BatchingShader;

	HXG_DECLSPEC extern Shader basic_shader;
	HXG_DECLSPEC extern Shader line_shader;
	HXG_DECLSPEC extern Shader font_shader;
	HXG_DECLSPEC extern Shader tex_vector_shader;
	HXG_DECLSPEC extern Shader poly_shader;
    HXG_DECLSPEC extern BatchingShader batching_shader_250;

	HXG_DECLSPEC extern std::string basic_vs;
	HXG_DECLSPEC extern std::string poly_vs;
	HXG_DECLSPEC extern std::string poly_fs;
	HXG_DECLSPEC extern std::string batching_vs;
	HXG_DECLSPEC extern std::string batching_fs;
	HXG_DECLSPEC extern std::string font_fs;
	HXG_DECLSPEC extern std::string line_fs;
	HXG_DECLSPEC extern std::string basic_fs;
	HXG_DECLSPEC extern std::string tex_vector_vs;
}

#endif
