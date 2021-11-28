#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

namespace Hexeng::Renderer
{

	class VertexBuffer
	{
	private:

		unsigned int m_id = 0;

	public:

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer() = default;

		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;

		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;
	};

}

#endif // !VERTEX_BUFFER_HPP