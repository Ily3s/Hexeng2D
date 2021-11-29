#ifndef INIT_PRESETS_HPP
#define INIT_PRESETS_HPP

#include "../Shader.hpp"
#include "../VertexLayout.hpp"
#include "../Uniform.hpp"

namespace Hexeng::Renderer::Presets
{

	extern Shader basic_shader;
	extern VertexLayout basic_vertex_layout;

	extern Vec2<float> transform;
	extern float zoom;

	extern Uniform<Vec2<float>> u_cam;
	extern Uniform<float> u_zoom;
	extern Uniform<Vec2<float>> u_transform;

	void init();

}

#endif // !INIT_PRESETS_HPP