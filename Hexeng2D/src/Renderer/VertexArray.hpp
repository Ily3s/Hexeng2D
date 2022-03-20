#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "VertexLayout.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC class VertexArray
	{

	private:

		unsigned int m_id;

	public:

		HXG_DECLSPEC VertexArray();
		HXG_DECLSPEC ~VertexArray();
		 
		HXG_DECLSPEC VertexArray(const VertexArray&) = delete;
		HXG_DECLSPEC VertexArray& operator=(const VertexArray&) = delete;
		 
		HXG_DECLSPEC VertexArray(VertexArray&&) noexcept;
		HXG_DECLSPEC VertexArray& operator=(VertexArray&&) noexcept;
		 
		HXG_DECLSPEC void tie(const VertexBuffer& vb, const VertexLayout& layout, const IndexBuffer& ib);
		HXG_DECLSPEC void tie(const VertexBuffer& vb, const VertexLayout& layout);
		HXG_DECLSPEC void tie(unsigned int vb, const VertexLayout& layout);
		 
		HXG_DECLSPEC void bind() const;
		HXG_DECLSPEC void unbind() const;

	};

}

#endif // !VERTEX_ARRAY_HPP