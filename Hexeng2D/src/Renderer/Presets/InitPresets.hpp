#ifndef INIT_PRESETS_HPP
#define INIT_PRESETS_HPP

#include "../../Macros.hpp"
#include "../Shader.hpp"
#include "../VertexLayout.hpp"
#include "../Uniform.hpp"

namespace Hexeng::Renderer::Presets
{

	HXG_DECLSPEC extern Shader basic_shader;
	HXG_DECLSPEC extern VertexLayout basic_vertex_layout;

	HXG_DECLSPEC extern Vec2<float> transform;
	HXG_DECLSPEC extern Uniform<Vec2<float>> u_transform;

	HXG_DECLSPEC void init();
	HXG_DECLSPEC void stop();

}

#endif // !INIT_PRESETS_HPP