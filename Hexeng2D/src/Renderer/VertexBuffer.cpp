#include "glad/glad.h"
#include "VertexBuffer.hpp"
#include "../Macros.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	VertexBuffer::VertexBuffer() = default;

	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		HXG_GL(glBindVertexArray(0));

		HXG_GL(glGenBuffers(1, &m_id));
		HXG_GL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		HXG_GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		HXG_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

		ToBeDelete(this, [this]() { this->~VertexBuffer(); });
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_id)
		{
			HXG_GL(glDeleteBuffers(1, &m_id));
			m_id = 0;
		}
	}

	void VertexBuffer::bind() const
	{
		HXG_GL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void VertexBuffer::unbind() const
	{
		HXG_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		: m_id(other.m_id)
	{
		other.m_id = 0;
		ToBeDelete(this, [this]() { this->~VertexBuffer(); });
		ToBeDelete::remove(&other);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		if (m_id)
			HXG_GL(glDeleteBuffers(1, &m_id));
		m_id = other.m_id;
		other.m_id = 0;

		ToBeDelete(this, [this]() { this->~VertexBuffer(); });
		ToBeDelete::remove(&other);

		return *this;
	}

}