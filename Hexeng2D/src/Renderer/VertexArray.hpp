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
	public:

		VertexArray() = default;
		~VertexArray();

		VertexArray(const VertexBuffer& vb, const VertexLayout& layout, const IndexBuffer& ib);
		VertexArray(const VertexBuffer& vb, const VertexLayout& layout);
		
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;
		
		void bind() const;
		void unbind() const;

		bool is_init() const;

	private:

		unsigned int m_id = 0;

	};

}

#endif // !VERTEX_ARRAY_HPP