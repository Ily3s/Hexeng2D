#ifndef MESH_HPP
#define MESH_HPP

#include "../Macros.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

#include <functional>

namespace Hexeng::Renderer
{

	HXG_DECLSPEC class Mesh
	{

	protected:

		VertexArray m_vao;
		VertexBuffer m_vb;
		const IndexBuffer* m_ib;
		Texture* m_texture;
		Shader* m_shader;

	public:

		std::function<void()> pre_render_event = nullptr;
		std::function<void()> post_render_event = nullptr;

		HXG_DECLSPEC Mesh();
		
		HXG_DECLSPEC Mesh(const float* vb, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader);
		
		HXG_DECLSPEC Mesh(Mesh&& moving) noexcept;
		HXG_DECLSPEC Mesh& operator=(Mesh&& moving) noexcept;
		
		inline const Texture* get_texture() const { return m_texture; }
		inline const VertexArray* get_vao() const { return &m_vao; }
		inline const Shader* get_shader() const { return m_shader; }
		inline const IndexBuffer* get_ib() const { return m_ib; }
		inline Texture*& access_texture() { return m_texture; }
		inline Shader*& access_shader() { return m_shader; }
		
		HXG_DECLSPEC virtual void draw();
		 
	};

}

#endif // !MESH_HPP