#include "VertexArray.hpp"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	VertexArray::VertexArray()
	{
		HXG_SGL(glGenVertexArrays(1, &m_id));
	}

	VertexArray::~VertexArray()
	{
		if (m_id)
		{
			HXG_SGL(glDeleteVertexArrays(1, &m_id));
			m_id = 0;
		}
	}

	void VertexArray::bind() const
	{
		HXG_SGL(glBindVertexArray(m_id));
	}

	void VertexArray::unbind() const
	{
		HXG_SGL(glBindVertexArray(0));
	}

	void VertexArray::tie(const VertexBuffer& vb, const VertexLayout& layout, const IndexBuffer& ib)
	{
		bind();

		vb.bind();
		layout.bind();
		ib.bind();

		unbind();

		vb.unbind();
		layout.unbind();
		ib.unbind();
	}

	void VertexArray::tie(const VertexBuffer& vb, const VertexLayout& layout)
	{
		bind();

		vb.bind();
		layout.bind();

		unbind();

		vb.unbind();
		layout.unbind();
	}

	void VertexArray::tie(unsigned int vb, const VertexLayout& layout)
	{
		bind();

		HXG_SGL(glBindBuffer(GL_ARRAY_BUFFER, vb));
		layout.bind();

		unbind();

		HXG_SGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		layout.unbind();
	}

	VertexArray::VertexArray(VertexArray&& va) noexcept
		: m_id(va.m_id)
	{
		va.m_id = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
	{
		if (m_id)
			HXG_SGL(glDeleteBuffers(1, &m_id));
		m_id = va.m_id;
		va.m_id = 0;
		return *this;
	}

}