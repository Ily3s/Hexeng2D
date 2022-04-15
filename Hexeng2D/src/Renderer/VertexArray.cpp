#include "VertexArray.hpp"
#include "../Macros.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	VertexArray::VertexArray()
	{
		HXG_GL(glGenVertexArrays(1, &m_id));
		ToBeDelete(this, [this]() { this->~VertexArray(); });
	}

	VertexArray::~VertexArray()
	{
		if (m_id)
		{
			HXG_GL(glDeleteVertexArrays(1, &m_id));
			m_id = 0;
		}
	}

	void VertexArray::bind() const
	{
		HXG_GL(glBindVertexArray(m_id));
	}

	void VertexArray::unbind() const
	{
		HXG_GL(glBindVertexArray(0));
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

		HXG_GL(glBindBuffer(GL_ARRAY_BUFFER, vb));
		layout.bind();

		unbind();

		HXG_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		layout.unbind();
	}

	VertexArray::VertexArray(VertexArray&& va) noexcept
		: m_id(va.m_id)
	{
		va.m_id = 0;
		ToBeDelete(this, [this]() { this->~VertexArray(); });
		ToBeDelete::remove(&va);
	}

	VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
	{
		if (m_id)
			HXG_GL(glDeleteBuffers(1, &m_id));
		m_id = va.m_id;
		va.m_id = 0;

		ToBeDelete::remove(&va);
		ToBeDelete(this, [this]() { this->~VertexArray(); });

		return *this;
	}

}