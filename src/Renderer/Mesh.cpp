#include "Mesh.hpp"

namespace Hexeng::Renderer
{

	Mesh::Mesh(const float* vb, const VertexLayout& layout, const unsigned int* ib, Texture* tex, Shader* shader)
		:	m_vb(vb, sizeof(vb)),
			m_ib(ib, GL_FLOAT, sizeof(ib) / sizeof(float)),
			m_texture(tex),
			m_shader(shader)
	{
		m_vao.tie(m_vb, layout, m_ib);
	}

	Mesh::Mesh(Mesh&& moving)
		:	m_vao(std::move(moving.m_vao)),
			m_texture(moving.m_texture),
			m_vb(std::move(moving.m_vb)),
			m_shader(moving.m_shader),
			m_ib(std::move(moving.m_ib)){}

	Mesh& Mesh::operator=(Mesh&& moving)
	{
		m_vao = std::move(moving.m_vao);
		m_texture = moving.m_texture;
		m_vb = std::move(moving.m_vb);
		m_shader = moving.m_shader;
		m_ib = std::move(moving.m_ib);

		return *this;
	}

}