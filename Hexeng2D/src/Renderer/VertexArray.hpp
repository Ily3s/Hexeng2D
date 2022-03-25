#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "VertexLayout.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC VertexArray
	{

	private:

		unsigned int m_id;

	public:

		VertexArray();
		~VertexArray();
		
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;
		
		void tie(const VertexBuffer& vb, const VertexLayout& layout, const IndexBuffer& ib);
		void tie(const VertexBuffer& vb, const VertexLayout& layout);
		void tie(unsigned int vb, const VertexLayout& layout);
		
		void bind() const;
		void unbind() const;

	};

}

#endif // !VERTEX_ARRAY_HPP