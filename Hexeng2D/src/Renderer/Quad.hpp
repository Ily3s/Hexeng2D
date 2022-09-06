#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include "../Macros.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "DefaultShaders.hpp"

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Quad : public Mesh
	{
	protected:

		static IndexBuffer s_index_buffer;
		static IndexBuffer s_edge_index_buffer;

		static ToBeInit init_components;

	public:

		static VertexLayout s_vertex_layout;

		Quad(const int* vertecies, const Vec2<int>& pos, Texture* texture, Shader* shader = &basic_shader);

		Quad() = default;

		Quad(Quad&&) noexcept;
		Quad& operator=(Quad&&) noexcept;

		~Quad() = default;
	};

	class HXG_DECLSPEC Rectangle : public virtual Quad
	{
	private :

		Vec2<int> m_min{0, 0}, m_max{0, 0}, m_size{0, 0};

	public:

		Rectangle(Vec2<int> pos, const Vec2<int>& size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);
		Rectangle(const Vec2<int>& pos, float size, Texture* texture, bool centerd = true, Shader* shader = &basic_shader);

		Rectangle() = default;
		~Rectangle() = default;

		Rectangle(Rectangle&&) noexcept;
		Rectangle& operator=(Rectangle&&) noexcept;

		inline Vec2<int> get_min() { return m_min; }
		inline Vec2<int> get_max() { return m_max; }
		inline Vec2<int> get_size() { return m_size; }
	};

	class HXG_DECLSPEC Square : public Rectangle
	{
	public:

		Square(const Vec2<int>& pos, int size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);
		Square(const Vec2<int>& pos, float size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);

		Square() = default;
		~Square() = default;

		Square(Square&&) noexcept;
		Square& operator=(Square&&) noexcept;
	};

	class HXG_DECLSPEC DebugQuad : public virtual Quad
	{
	public:

		DebugQuad(const int* vertecies, const Vec2<int>& pos, Shader* shader = &line_shader);

		DebugQuad() = default;

		DebugQuad(DebugQuad&&) noexcept;
		DebugQuad& operator=(DebugQuad&&) noexcept;
	};

	class HXG_DECLSPEC DebugRectangle : public virtual DebugQuad, public virtual Rectangle
	{
	public:

		DebugRectangle(const Vec2<int>& pos, const Vec2<int>& size, bool centered = true, Shader* shader = &line_shader);

		DebugRectangle() = default;

		DebugRectangle(DebugRectangle&&) noexcept;
		DebugRectangle& operator=(DebugRectangle&&) noexcept;
	};

	class HXG_DECLSPEC DebugSquare : public virtual DebugRectangle, public virtual Square
	{
	public:

		DebugSquare(const Vec2<int>& pos, int size, bool centered = false, Shader* shader = &line_shader);

		DebugSquare() = default;

		DebugSquare(DebugSquare&&) noexcept;
		DebugSquare& operator=(DebugSquare&&) noexcept;
	};

}

#endif // !BASIC_QUAD_HPP