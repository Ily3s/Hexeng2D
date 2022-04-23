#ifndef PRESETS_HPP
#define PRESETS_HPP

#include "../../Macros.hpp"
#include "../Shader.hpp"
#include "../VertexLayout.hpp"
#include "../Uniform.hpp"
#include "Color.hpp"

namespace Hexeng::Renderer::Presets
{

	HXG_DECLSPEC extern Shader basic_shader;
	HXG_DECLSPEC extern Shader line_shader;
	HXG_DECLSPEC extern Shader font_shader;
	HXG_DECLSPEC extern Uniform<Color3> u_color;
	HXG_DECLSPEC extern Color3 color;
	HXG_DECLSPEC extern VertexLayout basic_vertex_layout;

	HXG_DECLSPEC void stop();

}

#endif // !PRESETS_HPP