#include "Mesh.hpp"

namespace Hexeng::Renderer
{

	Mesh::Mesh() = default;

	Mesh::Mesh(const float* vb, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader)
		:	m_vb(vb, sizeof(vb)),
			m_ib(ib),
			m_texture(tex),
			m_shader(shader)
	{
		m_vao.tie(m_vb, layout, *m_ib);
	}

	Mesh::Mesh(Mesh&& moving) noexcept
		:	m_vao(std::move(moving.m_vao)),
			m_texture(moving.m_texture),
			m_vb(std::move(moving.m_vb)),
			m_shader(moving.m_shader),
			m_ib(std::move(moving.m_ib)),
			uniforms(std::move(moving.uniforms)) {}

	Mesh& Mesh::operator=(Mesh&& moving) noexcept
	{
		m_vao = std::move(moving.m_vao);
		m_texture = moving.m_texture;
		m_vb = std::move(moving.m_vb);
		m_shader = moving.m_shader;
		m_ib = std::move(moving.m_ib);
		uniforms = std::move(moving.uniforms);

		return *this;
	}

	void Mesh::draw()
	{
		m_shader->bind();

		for (auto& [uniform, value] : uniforms)
		{
			uniform->refresh(m_shader, value);
		}

		m_texture->bind();
		m_vao.bind();
		HXG_GL(glDrawElements(GL_TRIANGLES, m_ib->get_count(), m_ib->get_type(), nullptr));

		for (auto& [uniform, value] : uniforms)
		{
			uniform->refresh(m_shader);
		}
	}

}