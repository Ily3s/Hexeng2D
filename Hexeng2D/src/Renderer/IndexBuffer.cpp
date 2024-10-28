#ifndef INDEX_BUFFER_TPP
#define INDEX_BUFFER_TPP

#include "glad/glad.h"
#include "IndexBuffer.hpp"
#include "../Macros.hpp"
#include "VertexLayout.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	IndexBuffer::IndexBuffer() = default;

	IndexBuffer::IndexBuffer(const void* data, unsigned int type, unsigned int count)
		: m_count(count),
		m_type(type)
	{
		HXG_GL(glBindVertexArray(0));

		HXG_GL(glGenBuffers(1, &m_id));
		HXG_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
		HXG_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof_type(type), data, GL_STATIC_DRAW));
		HXG_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		ToBeDelete(this, [this]() { this->~IndexBuffer(); });
	}

	IndexBuffer::~IndexBuffer()
	{
		if (m_id)
		{
			HXG_GL(glDeleteBuffers(1, &m_id));
			m_id = 0;
		}
	}

	void IndexBuffer::bind() const
	{
		HXG_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	}

	void IndexBuffer::unbind() const
	{
		HXG_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
		: m_id(other.m_id),
		m_count(other.m_count),
		m_type(other.m_type)
	{
		other.m_id = 0;
		ToBeDelete(this, [this]() { this->~IndexBuffer(); });
		ToBeDelete::remove(&other);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		if (m_id)
			HXG_GL(glDeleteBuffers(1, &m_id));
		m_id = other.m_id;
		m_count = other.m_count;
		m_type = other.m_type;
		other.m_id = 0;

		ToBeDelete::remove(&other);
		ToBeDelete(this, [this]() { this->~IndexBuffer(); });

		return *this;
	}

}

#endif