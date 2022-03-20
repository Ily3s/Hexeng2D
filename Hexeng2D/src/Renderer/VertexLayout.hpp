#ifndef VERTEX_LAYOUT_HPP
#define VERTEX_LAYOUT_HPP

#include <vector>
#include "glad/glad.h"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC struct VertexElement
	{

		unsigned int count;
		unsigned int type;

	};

	static unsigned int sizeof_type(unsigned int type)
	{
		switch (type)
		{
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_INT:			return sizeof(GLint);
		case GL_SHORT:			return sizeof(GLshort);
		case GL_UNSIGNED_SHORT: return sizeof(GLushort);
		default: throw std::exception();
		}
	}

	

	HXG_DECLSPEC class VertexLayout
	{
	private:

		std::vector<VertexElement> m_elements;

		unsigned int m_stride = 0;

	public:

		VertexLayout() = default;

		VertexLayout(const std::vector<VertexElement>& vertex_elements)
		{
			m_elements = vertex_elements;
			for (VertexElement ve : m_elements)
			{
				m_stride += sizeof_type(ve.type) * ve.count;
			}
		}

		VertexLayout(VertexLayout&& moving) noexcept
			:	m_elements (std::move(moving.m_elements)),
				m_stride(moving.m_stride) {}

		VertexLayout& operator=(VertexLayout&& moving) noexcept
		{
			m_elements = std::move(moving.m_elements);
			m_stride = moving.m_stride;
			return *this;
		}

		void add(unsigned int count, unsigned int type)
		{
			m_elements.push_back({ count, type });
			m_stride += sizeof_type(type) * count;
		}

		inline const std::vector<VertexElement>& get_elements() const { return m_elements; }
		inline const unsigned int get_stride() const { return m_stride; }

		void bind() const
		{
			unsigned int offset = 0;
			for (unsigned int i = 0; i < m_elements.size(); ++i)
			{
				const auto& element = m_elements[i];
				HXG_GL(glEnableVertexAttribArray(i));
				HXG_GL(glVertexAttribPointer(i, element.count, element.type, GL_FALSE, m_stride, (const void*)offset));
				offset += element.count * sizeof_type(element.type);
			}
		}

		void unbind() const
		{
			for (unsigned int i = 0; i < m_elements.size(); ++i)
			{
				HXG_GL(glDisableVertexAttribArray(i));
			}
		}
	};

}


#endif // !VERTEX_LAYOUT_HPP