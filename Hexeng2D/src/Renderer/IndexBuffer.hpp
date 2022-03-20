#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC class IndexBuffer
	{
	private:

		unsigned int m_id = 0;

		unsigned int m_count = 0;

		unsigned int m_type = 0;

	public:

		HXG_DECLSPEC IndexBuffer();
		 
		HXG_DECLSPEC IndexBuffer(const void* data, unsigned int type, unsigned int count);
		HXG_DECLSPEC ~IndexBuffer();
		 
		HXG_DECLSPEC IndexBuffer(const IndexBuffer&) = delete;
		HXG_DECLSPEC IndexBuffer& operator=(const IndexBuffer&) = delete;
		 
		HXG_DECLSPEC IndexBuffer(IndexBuffer&&) noexcept;
		HXG_DECLSPEC IndexBuffer& operator=(IndexBuffer&&) noexcept;
		  
		HXG_DECLSPEC void bind() const;
		HXG_DECLSPEC void unbind() const;
		 
		inline unsigned int get_count() const { return m_count; }
		inline unsigned int get_type() const { return m_type; }
	};

}

#endif // !INDEX_BUFFER_HPP