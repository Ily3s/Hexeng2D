#ifndef DEFAULT_SHADERS_HPP
#define DEFAULT_SHADERS_HPP

#include "../Macros.hpp"
#include "Shader.hpp"
#include "Uniform.hpp"
#include "../Color.hpp"

namespace Hexeng::Renderer
{
	HXG_DECLSPEC extern Shader basic_shader;
	HXG_DECLSPEC extern Shader line_shader;
	HXG_DECLSPEC extern Shader font_shader;
	HXG_DECLSPEC extern Shader tex_vector_shader;
	HXG_DECLSPEC extern Shader batching_shader;
	HXG_DECLSPEC extern Shader poly_shader;
}

#endif