#include "Quad.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "../Hexeng.hpp"

namespace Hexeng::Renderer
{

	IndexBuffer Quad::s_index_buffer;
	IndexBuffer Quad::s_edge_index_buffer;
	VertexLayout Quad::s_vertex_layout;

	ToBeInit Quad::s_init_components
	{ []() {
		unsigned char index_buffer[]
		{
			0, 1, 2,
			2, 3, 0
		};

		unsigned char edge_index_buffer[]
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			0, 2
		};

		s_vertex_layout = VertexLayout({ { 2, GL_FLOAT }, { 2, GL_FLOAT } });
		s_index_buffer = IndexBuffer(index_buffer, GL_UNSIGNED_BYTE, 6);
		s_edge_index_buffer = IndexBuffer(edge_index_buffer, GL_UNSIGNED_BYTE, 10);
	}, 1 };

	Quad::Quad(Vec2<float> pos, const Vec2<float>& size, Texture* texture, bool centered, Shader* shader)
		: m_size(size)
	{
		Vec2<float> relative_pos = { 0, 0 };
		if (centered)
			relative_pos -= (size / 2);

		m_min = relative_pos + pos;
		m_max = m_min + size;

		float vertecies[]
		{
			relative_pos.x,			    relative_pos.y,			0.0f, 0.0f,
			relative_pos.x,			    relative_pos.y + size.y,	0.0f, 1.0f,
			relative_pos.x + size.x,	relative_pos.y + size.y,	1.0f, 1.0f,
			relative_pos.x + size.x,	relative_pos.y,			1.0f, 0.0f
		};

		this->Mesh::operator=({ vertecies, 4 * 4 * sizeof(float), pos, s_vertex_layout, &s_index_buffer, texture, shader });
	}

	DebugQuad::DebugQuad(const Vec2<float>& pos, const Vec2<float>& size, bool centered, Shader* shader)
		: Quad(pos, size, nullptr, centered, shader)
	{
		m_type = GL_LINES;
		m_ib = &s_edge_index_buffer;
		m_vao = { m_vb, s_vertex_layout, s_edge_index_buffer };
	}

	Quad::Quad(const Vec2<float>& pos, float size, Texture* texture, bool centered, Shader* shader)
	{
		this->Quad::operator=({pos, texture->get_size() * size, texture, centered, shader});
	}

	Square::Square(const Vec2<float>& pos, float size, Texture* texture, bool centered, Shader* shader)
		: Quad(pos, { size, size }, texture, centered, shader) {}

	DebugSquare::DebugSquare(const Vec2<float>& pos, float size, bool centered, Shader* shader)
		: Square(pos, size, nullptr, centered, shader)
	{
		m_type = GL_LINES;
		m_ib = &s_edge_index_buffer;
		m_vao = { m_vb, s_vertex_layout, s_edge_index_buffer };
	}

	DebugQuad& DebugQuad::operator=(DebugQuad&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		return *this;
	}

	DebugSquare& DebugSquare::operator=(DebugSquare&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		return *this;
	}

	Quad& Quad::operator=(Quad&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		m_min = other.m_min;
		m_max = other.m_max;
		m_size = other.m_size;

		return *this;
	}

	Square& Square::operator=(Square&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		return *this;
	}

	DebugQuad::DebugQuad(DebugQuad&& other) noexcept
		: Mesh(std::move(other)) {}

	DebugSquare::DebugSquare(DebugSquare&& other) noexcept
		: Mesh(std::move(other)) {}

	Square::Square(Square&& other) noexcept
		: Mesh(std::move(other)) {}

	Quad::Quad(Quad&& other) noexcept
		: Mesh(std::move(other)),
		m_min(other.m_min),
		m_max(other.m_max),
		m_size(other.m_size) {}
}
