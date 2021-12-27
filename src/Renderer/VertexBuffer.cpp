#include "glad/glad.h"
#include "VertexBuffer.hpp"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		HXG_SGL(glGenBuffers(1, &m_id));
		HXG_SGL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		HXG_SGL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		HXG_SGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_id)
		{
			HXG_SGL(glDeleteBuffers(1, &m_id));
			m_id = 0;
		}
	}

	void VertexBuffer::bind() const
	{
		HXG_SGL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void VertexBuffer::unbind() const
	{
		HXG_SGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		: m_id(other.m_id)
	{
		other.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		if (m_id)
			HXG_SGL(glDeleteBuffers(1, &m_id));
		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}

}