#include "BasicQuad.hpp"
#include "../../Functions.hpp"
#include "InitPresets.hpp"

#include <exception>

namespace Hexeng::Renderer::Presets
{

	IndexBuffer BasicQuad::s_index_buffer;

	void BasicQuad::verify_uniforms(Shader* shader)
	{
		auto it_cam = u_cam.shader_list.find(shader);
		auto it_zoom = u_zoom.shader_list.find(shader);
		auto it_transform = u_transform.shader_list.find(shader);

		if(it_cam == u_cam.shader_list.end())
			throw(std::exception("cam uniform excepted"));

		if(it_zoom == u_zoom.shader_list.end())
			throw(std::exception("zoom uniform excepted"));

		if(it_transform == u_transform.shader_list.end())
			throw(std::exception("transform uniform excepted"));
	}

	BasicQuad::BasicQuad(const int* vertecies, Texture* texture, const Vec2<int>& pos, Shader* shader)
		: position(pos)
	{
		verify_uniforms(shader);

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
	}

	BasicQuad::BasicQuad(BasicQuad&& moving) noexcept
		: Mesh(std::move(moving)), position(moving.position) {}

	BasicQuad& BasicQuad::operator=(BasicQuad&& moving) noexcept
	{
		Mesh::operator=(std::move(moving));
		position = moving.position;
		return *this;
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
		transform = toCoord(position);
		u_transform.refresh(m_shader);
		Mesh::draw();
	}

	BasicQuad::BasicQuad() = default;

	BasicRectangle::BasicRectangle(Vec2<int> pos, const Vec2<int>& size, Texture* texture, bool centered, Shader* shader)
	{
		verify_uniforms(shader);

		if (centered)
			pos -= (size/2);

		float vertex_b[]
		{
			toX(pos.x) ,			toY(pos.y) ,			0.0f, 0.0f,		// 0
			toX(pos.x) ,			toY(pos.y + size.y) ,	0.0f, 1.0f,		// 1	
			toX(pos.x + size.x) ,	toY(pos.y + size.y) ,	1.0f, 1.0f,		// 2	
			toX(pos.x + size.x) ,	toY(pos.y) ,			1.0f, 0.0f		// 3
		};

		m_texture = texture;
		m_ib = &s_index_buffer;
		m_vb = VertexBuffer(vertex_b, sizeof(vertex_b));
		m_shader = shader;
		m_vao.tie(m_vb, basic_vertex_layout, s_index_buffer);
	}

	BasicRectangle::BasicRectangle(const Vec2<int>& pos, float size, Texture* texture, bool centered, Shader* shader)
	{
		this->BasicRectangle::BasicRectangle(pos, texture->size() * size, texture, centered, shader);
	}

	BasicRectangle::BasicRectangle() = default;

	BasicSquare::BasicSquare(const Vec2<int>& pos, int size, Texture* texture, bool centered, Shader* shader)
		: BasicRectangle(pos, {size, size}, texture, centered, shader) {}

	BasicSquare::BasicSquare(const Vec2<int>& pos, float size, Texture* texture, bool centered, Shader* shader)
		: BasicRectangle(pos, size, texture, centered, shader)
	{
		if (texture->get_height() != texture->get_width())
			throw(std::exception("excepted texure to be a square"));
	}

	BasicSquare::BasicSquare() = default;
}