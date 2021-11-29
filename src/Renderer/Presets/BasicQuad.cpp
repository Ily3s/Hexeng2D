#include "BasicQuad.hpp"
#include "../../Functions.hpp"
#include "InitPresets.hpp"

#include <exception>

namespace Hexeng::Renderer::Presets
{

	BasicQuad::BasicQuad(const int* positions, Texture* texture, Shader* shader)
	{
		float pos[]
		{
			toX(positions[0]), toX(positions[1]), 0.0f, 0.0f,
			toX(positions[2]), toX(positions[3]), 0.0f, 1.0f,
			toX(positions[4]), toX(positions[5]), 1.0f, 1.0f,
			toX(positions[6]), toX(positions[7]), 1.0f, 0.0f
		};

		m_vb = VertexBuffer(pos, sizeof(pos));
		m_shader = shader;
		m_vao.tie(m_vb, basic_vertex_layout, s_index_buffer);
	}

	void BasicQuad::init()
	{
		unsigned char index_buffer[]
		{
			0, 1, 2,
			2, 3, 0
		};

		s_index_buffer = IndexBuffer(index_buffer, GL_UNSIGNED_BYTE, 6);
	}

	void BasicQuad::draw()
	{
		u_transform.refresh();
		Mesh::draw();
	}

	BasicRectangle::BasicRectangle(const Vec2<int>& origin, Vec2<int> size, Texture* texture, Shader* shader)
	{
		float pos[]
		{
			toX(origin.x) ,				toY(origin.y) ,				0.0f, 0.0f,		// 0
			toX(origin.x) ,				toY(origin.y + size.y) ,	0.0f, 1.0f,		// 1	
			toX(origin.x + size.x) ,	toY(origin.y + size.y) ,	1.0f, 1.0f,		// 2	
			toX(origin.x + size.x) ,	toY(origin.y) ,				1.0f, 0.0f		// 3
		};

		m_vb = VertexBuffer(pos, sizeof(pos));
		m_shader = shader;
		m_vao.tie(m_vb, basic_vertex_layout, s_index_buffer);
	}

	BasicRectangle::BasicRectangle(const Vec2<int>& origin, float size, Texture* texture, Shader* shader)
	{
		this->BasicRectangle::BasicRectangle(origin, texture->size() * size, texture, shader);
	}

	BasicSquare::BasicSquare(const Vec2<int>& origin, int size, Texture* texture, Shader* shader)
	{
		this->BasicRectangle::BasicRectangle(origin, {size, size}, texture, shader);
	}

	BasicSquare::BasicSquare(const Vec2<int>& origin, float size, Texture* texture, Shader* shader)
	{
		if (texture->get_height() != texture->get_width())
			throw(std::exception("excepted texure to be a square"));
		this->BasicRectangle::BasicRectangle(origin, size, texture, shader);
	}
}