#ifndef MESH_HPP
#define MESH_HPP

#include "../Macros.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Uniform.hpp"

#include <functional>

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Mesh
	{

	protected:

		VertexArray m_vao;
		VertexBuffer m_vb;
		const IndexBuffer* m_ib;
		Texture* m_texture;
		Shader* m_shader;

	public:

		Mesh();

		Mesh(const float* vb, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader);

		Mesh(Mesh&& moving) noexcept;
		Mesh& operator=(Mesh&& moving) noexcept;

		inline const Texture* get_texture() const { return m_texture; }
		inline const VertexArray* get_vao() const { return &m_vao; }
		inline const Shader* get_shader() const { return m_shader; }
		inline const IndexBuffer* get_ib() const { return m_ib; }
		inline Texture*& access_texture() { return m_texture; }
		inline Shader*& access_shader() { return m_shader; }

		virtual void draw();

		// for obscure reasons, unordered_map doesn't work here (error when destructor get called)
		std::vector<std::pair<UniformInterface*, void*>> uniforms;
	};

}

#endif // !MESH_HPP