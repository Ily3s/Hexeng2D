#include "Mesh.hpp"

namespace Hexeng::Renderer
{

	Mesh::Mesh(const float* vb, const VertexLayout& layout, const unsigned int* ib, Texture* tex, Shader* shader)
		:	m_vb(vb, sizeof(vb)),
			m_ib(ib, GL_FLOAT, sizeof(*ib) / sizeof(float)),
			m_texture(tex),
			m_shader(shader)
	{
		m_vao.tie(m_vb, layout, m_ib);
	}

	Mesh::Mesh(Mesh&& moving) noexcept
		:	m_vao(std::move(moving.m_vao)),
			m_texture(moving.m_texture),
			m_vb(std::move(moving.m_vb)),
			m_shader(moving.m_shader),
			m_ib(std::move(moving.m_ib)),
			pre_render_event(moving.pre_render_event),
			post_render_event(moving.post_render_event) {}

	Mesh& Mesh::operator=(Mesh&& moving) noexcept
	{
		m_vao = std::move(moving.m_vao);
		m_texture = moving.m_texture;
		m_vb = std::move(moving.m_vb);
		m_shader = moving.m_shader;
		m_ib = std::move(moving.m_ib);
		pre_render_event = moving.pre_render_event;
		post_render_event = moving.post_render_event;

		return *this;
	}

	void Mesh::draw()
	{

		if (pre_render_event)
			pre_render_event();

		m_texture->bind();
		m_vao.bind();
		m_shader->bind();
		HXG_SGL(glDrawElements(GL_TRIANGLES, m_ib.get_count(), m_ib.get_type(), nullptr));

		if (post_render_event)
			post_render_event();

	}

}