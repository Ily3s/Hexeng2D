#include "Mesh.hpp"
#include "../Functions.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	Mesh::Mesh() = default;

	Mesh::Mesh(const float* vb, const Vec2<int>& pos, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader, GLenum type)
		:	m_vb(vb, sizeof(vb)),
			m_ib(ib),
			m_texture(tex),
			m_shader(shader),
			m_type(type),
			position(pos)
	{
		m_vao.tie(m_vb, layout, *m_ib);
		uniforms.push_back({ &u_transform, &transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
	}

	Mesh::Mesh(Mesh&& moving) noexcept
		:	m_vao(std::move(moving.m_vao)),
			m_texture(moving.m_texture),
			m_vb(std::move(moving.m_vb)),
			m_shader(moving.m_shader),
			m_ib(std::move(moving.m_ib)),
			uniforms(std::move(moving.uniforms)),
			m_type(moving.m_type),
			position(moving.position),
			transform(moving.transform),
			rotation(moving.rotation) {}

	Mesh& Mesh::operator=(Mesh&& moving) noexcept
	{
		m_vao = std::move(moving.m_vao);
		m_texture = moving.m_texture;
		m_vb = std::move(moving.m_vb);
		m_shader = moving.m_shader;
		m_ib = std::move(moving.m_ib);
		uniforms = std::move(moving.uniforms);
		m_type = moving.m_type;
		position = moving.position;
		transform = moving.transform;
		rotation = moving.rotation;

		return *this;
	}

	void Mesh::draw()
	{
		m_shader->bind();

		update_position();

		for (auto& [uniform, value] : uniforms)
			uniform->refresh(m_shader, value);

		if (m_texture)
		{
			m_texture->bind();
		}

		m_vao.bind();

		HXG_GL(glDrawElements(m_type, m_ib->get_count(), m_ib->get_type(), nullptr));

		for (auto& [uniform, value] : uniforms)
			uniform->refresh(m_shader);
	}

	void Mesh::update_position()
	{
		transform = toCoord(position);
	}

}