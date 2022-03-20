#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC class VertexBuffer
	{
	private:

		unsigned int m_id = 0;

	public:

		HXG_DECLSPEC VertexBuffer(const VertexBuffer&) = delete;
		HXG_DECLSPEC VertexBuffer& operator=(const VertexBuffer&) = delete;
		 
		HXG_DECLSPEC VertexBuffer();
		 
		HXG_DECLSPEC VertexBuffer(VertexBuffer&&) noexcept;
		HXG_DECLSPEC VertexBuffer& operator=(VertexBuffer&&) noexcept;
		HXG_DECLSPEC 
		HXG_DECLSPEC VertexBuffer(const void* data, unsigned int size);
		HXG_DECLSPEC ~VertexBuffer();
		 
		HXG_DECLSPEC void bind() const;
		HXG_DECLSPEC void unbind() const;
	};

}

#endif // !VERTEX_BUFFER_HPP