#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Mesh.hpp"
#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "Layer.hpp"
#include "Uniform.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC void init();

	HXG_DECLSPEC void stop();

	HXG_DECLSPEC void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

	HXG_DECLSPEC void clear();

	HXG_DECLSPEC void refresh_uniforms();

	HXG_DECLSPEC extern void draw(const Layer& layer);

	HXG_DECLSPEC extern void draw_scene(unsigned int scene_parameter);

	HXG_DECLSPEC extern void draw_current_scene();

	HXG_DECLSPEC extern Vec2<float> transform;
	HXG_DECLSPEC extern Renderer::Uniform<Vec2<float>> u_transform;

}

#endif // !RENDERER_HPP