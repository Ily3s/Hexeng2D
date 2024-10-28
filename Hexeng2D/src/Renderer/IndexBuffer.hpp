#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC IndexBuffer
	{
	private:

		unsigned int m_id = 0;

		unsigned int m_count = 0;

		unsigned int m_type = 0;

	public:

		IndexBuffer();
		
		IndexBuffer(const void* data, unsigned int type, unsigned int count);
		~IndexBuffer();
		
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		
		IndexBuffer(IndexBuffer&&) noexcept;
		IndexBuffer& operator=(IndexBuffer&&) noexcept;
		
		void bind() const;
		void unbind() const;
		 
		inline unsigned int get_count() const { return m_count; }
		inline unsigned int get_type() const { return m_type; }
	};

}

#endif // !INDEX_BUFFER_HPP