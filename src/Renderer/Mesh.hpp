#ifndef MESH_HPP
#define MESH_HPP

#include "VertexArray.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

namespace Hexeng::Renderer
{

	class Mesh
	{

	protected:

		VertexArray m_vao;
		VertexBuffer m_vb;
		IndexBuffer m_ib;
		Texture* m_texture;
		Shader* m_shader;

	public:

		Mesh() = default;

		Mesh(const float* vb, const VertexLayout& layout, const unsigned int* ib, Texture* tex, Shader* shader);

		Mesh(Mesh&& moving) noexcept;
		Mesh& operator=(Mesh&& moving) noexcept;

		inline const Texture* get_texture() const { return m_texture; }
		inline const VertexArray* get_vao() const { return &m_vao; }
		inline const Shader* get_shader() const { return m_shader; }
		inline const IndexBuffer& get_ib() const { return m_ib; }
		inline Texture*& access_texture() { return m_texture; }
		inline Shader*& access_shader() { return m_shader; }

		virtual void draw();

	};

}

#endif // !MESH_HPP