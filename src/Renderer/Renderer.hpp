#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Mesh.hpp"
#include "../Vectors.hpp"
#include "Layer.hpp"

namespace Hexeng::Renderer
{

	void init();

	void stop();

	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

	void draw(const Mesh& mesh);

	void clear();

	void refresh_uniforms();

	extern void draw(const Layer& layer);

	extern void draw_scene(unsigned int scene_parameter);

	extern void draw_current_scene();

	extern Shader basic_shader;

}

#endif // !RENDERER_HPP