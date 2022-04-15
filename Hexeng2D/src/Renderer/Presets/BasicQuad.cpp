#include "BasicQuad.hpp"
#include "../Camera.hpp"
#include "../Renderer.hpp"
#include "../../Hexeng.hpp"

#include <exception>

namespace Hexeng::Renderer::Presets
{

	IndexBuffer BasicQuad::s_index_buffer;
	IndexBuffer BasicQuad::s_edge_index_buffer;

	BasicQuad::BasicQuad(const int* vertecies, const Vec2<int>& pos, Texture* texture, Shader* shader)
	{
		float vertex_b[]
		{
			toX(vertecies[0]), toX(vertecies[1]), 0.0f, 0.0f,
			toX(vertecies[2]), toX(vertecies[3]), 0.0f, 1.0f,
			toX(vertecies[4]), toX(vertecies[5]), 1.0f, 1.0f,
			toX(vertecies[6]), toX(vertecies[7]), 1.0f, 0.0f
		};

		m_ib = &s_index_buffer;
		m_vb = VertexBuffer(vertex_b, sizeof(vertex_b));
		m_shader = shader;
		m_vao.tie(m_vb, basic_vertex_layout, s_index_buffer);
		position = pos;
		uniforms.push_back({ &u_transform, &transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
	}

	void BasicQuad::init()
	{
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

		s_index_buffer = IndexBuffer(index_buffer, GL_UNSIGNED_BYTE, 6);
		s_edge_index_buffer = IndexBuffer(edge_index_buffer, GL_UNSIGNED_BYTE, 10);
	}

	BasicRectangle::BasicRectangle(Vec2<int> pos, const Vec2<int>& size, Texture* texture, bool centered, Shader* shader)
	{
		Vec2<int> relative_pos = { 0, 0 };
		if (centered)
			relative_pos -= (size / 2);

		float vertex_b[]
		{
			toX(relative_pos.x) ,			toY(relative_pos.y) ,			0.0f, 0.0f,		// 0
			toX(relative_pos.x) ,			toY(relative_pos.y + size.y) ,	0.0f, 1.0f,		// 1	
			toX(relative_pos.x + size.x) ,	toY(relative_pos.y + size.y) ,	1.0f, 1.0f,		// 2	
			toX(relative_pos.x + size.x) ,	toY(relative_pos.y) ,			1.0f, 0.0f		// 3
		};

		m_texture = texture;
		m_ib = &s_index_buffer;
		m_vb = VertexBuffer(vertex_b, sizeof(vertex_b));
		m_shader = shader;
		m_vao.tie(m_vb, basic_vertex_layout, s_index_buffer);
		position = pos;
		uniforms.push_back({ &u_transform, &transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
	}

	DebugRectangle::DebugRectangle(const Vec2<int>& pos, const Vec2<int>& size, bool centered, Shader* shader)
		: BasicRectangle(pos, size, nullptr, centered, shader)
	{
		m_type = GL_LINES;
		m_ib = &s_edge_index_buffer;
		m_vao.tie(m_vb, basic_vertex_layout, s_edge_index_buffer);
	}

	BasicRectangle::BasicRectangle(const Vec2<int>& pos, float size, Texture* texture, bool centered, Shader* shader)
	{
		this->BasicRectangle::BasicRectangle(pos, texture->size() * size, texture, centered, shader);
	}

	BasicSquare::BasicSquare(const Vec2<int>& pos, int size, Texture* texture, bool centered, Shader* shader)
		: BasicRectangle(pos, { size, size }, texture, centered, shader) {}

	BasicSquare::BasicSquare(const Vec2<int>& pos, float size, Texture* texture, bool centered, Shader* shader)
		: BasicRectangle(pos, size, texture, centered, shader)
	{
		if (texture && texture->get_height() != texture->get_width())
			throw(std::exception("excepted texure to be a square"));
	}

	DebugSquare::DebugSquare(const Vec2<int>& pos, int size, bool centered, Shader* shader)
		: BasicSquare(pos, size, nullptr, centered, shader)
	{
		m_type = GL_LINES;
		m_ib = &s_edge_index_buffer;
		m_vao.tie(m_vb, basic_vertex_layout, s_edge_index_buffer);
	}

	DebugQuad::DebugQuad(const int* vertecies, const Vec2<int>& pos, Shader* shader)
		: BasicQuad(vertecies, pos, nullptr, shader)
	{
		m_type = GL_LINES;
		m_ib = &s_edge_index_buffer;
		m_vao.tie(m_vb, basic_vertex_layout, s_edge_index_buffer);
	}

	DebugQuad& DebugQuad::operator=(DebugQuad&& other) noexcept
	{
		BasicQuad::operator=(std::move(other));
		return *this;
	}

	DebugRectangle& DebugRectangle::operator=(DebugRectangle&& other) noexcept
	{
		BasicQuad::operator=(std::move(other));
		return *this;
	}

	DebugSquare& DebugSquare::operator=(DebugSquare&& other) noexcept
	{
		BasicQuad::operator=(std::move(other));
		return *this;
	}

	BasicRectangle& BasicRectangle::operator=(BasicRectangle&& other) noexcept
	{
		BasicQuad::operator=(std::move(other));
		return *this;
	}

	BasicSquare& BasicSquare::operator=(BasicSquare&& other) noexcept
	{
		BasicQuad::operator=(std::move(other));
		return *this;
	}

	DebugQuad::DebugQuad(DebugQuad&& other) noexcept
		: BasicQuad(std::move(other)) {}

	DebugRectangle::DebugRectangle(DebugRectangle&& other) noexcept
		: BasicQuad(std::move(other)) {}

	DebugSquare::DebugSquare(DebugSquare&& other) noexcept
		: BasicQuad(std::move(other)) {}

	BasicSquare::BasicSquare(BasicSquare&& other) noexcept
		: BasicQuad(std::move(other)) {}

	BasicRectangle::BasicRectangle(BasicRectangle&& other) noexcept
		: BasicQuad(std::move(other)) {}

	BasicQuad::BasicQuad(BasicQuad&& moving) noexcept
		: Mesh(std::move(moving)) {}

	BasicQuad& BasicQuad::operator=(BasicQuad&& moving) noexcept
	{
		Mesh::operator=(std::move(moving));
		return *this;
	}
}