#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include <array>

#include "../Macros.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "DefaultShaders.hpp"

namespace Hexeng::Renderer
{

	// bug : obtuse angles

	class HXG_DECLSPEC Quad : public Mesh
	{
	public:

		static VertexLayout vertex_layout;

		/** @param vertecies The four vertecies positions relative to pos. Make the vertecies clockwise.
		* @param The position of the quad in the Hexeng2D coordinate system.
		* @note If you want to make a custom shader for the quad,
		* take a look at DefaultShaders .glsl/.hpp/.cpp, copy paste basic_shader and modify it as you want.
		*/
		Quad(const std::array<Vec2<int>, 4>& vertecies, const Vec2<int>& pos, Texture* texture, Shader* shader = &basic_shader);

		Quad() = default;

		Quad(Quad&&) noexcept;
		Quad& operator=(Quad&&) noexcept;

		~Quad() = default;

	protected:

		static IndexBuffer s_index_buffer;
		static IndexBuffer s_edge_index_buffer;

		static ToBeInit init_components;
	};

	class HXG_DECLSPEC Rectangle : public virtual Quad
	{
	public:

		/**
		* @param pos The position of the Rectangle.
		* @param size The dimentions (lenght, width) of the Rectangle.
		* @param centered If set to true, pos is the center of the rectangle, if set to false, pos is the bottom left of the rectangle.
		* @note If you want to make a custom shader for the quad,
		* take a look at DefaultShaders .glsl/.hpp/.cpp, copy paste basic_shader and modify it as you want.
		* */
		Rectangle(Vec2<int> pos, const Vec2<int>& size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);

		/// @brief The same as Rectangle(Vec2<int>, const Vec2<int>&, Texture*, bool, Shader*) except the size is a multiplier of the texture size.
		Rectangle(const Vec2<int>& pos, float size, Texture* texture, bool centerd = true, Shader* shader = &basic_shader);

		Rectangle() = default;
		~Rectangle() = default;

		Rectangle(Rectangle&&) noexcept;
		Rectangle& operator=(Rectangle&&) noexcept;

	private:

		Vec2<int> m_min{ 0, 0 }, m_max{ 0, 0 }, m_size{ 0, 0 };

	public:

		/// @brief Get the bottom left coordinates
		inline Vec2<int> get_min() { return m_min; }
		/// @brief Get the top right coordinates
		inline Vec2<int> get_max() { return m_max; }
		/// @brief get_max() - get_min()
		inline Vec2<int> get_size() { return m_size; }
	};

	class HXG_DECLSPEC Square : public Rectangle
	{
	public:

		/// @brief The same as Rectangle(Vec2<int>, const Vec2<int>&, Texture*, bool, Shader*) except the size is the lenght of the square.
		Square(const Vec2<int>& pos, int size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);

		/// @brief Exactly the same as Rectangle(const Vec2<int>&, float, Texture*, bool, Shader*)
		Square(const Vec2<int>& pos, float size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);

		Square() = default;
		~Square() = default;

		Square(Square&&) noexcept;
		Square& operator=(Square&&) noexcept;
	};

	/// @brief Meant to be used internally by the engine only.
	class HXG_DECLSPEC DebugQuad : public virtual Quad
	{
	public:

		DebugQuad(const std::array<Vec2<int>, 4>& vertecies, const Vec2<int>& pos, Shader* shader = &line_shader);

		DebugQuad() = default;

		DebugQuad(DebugQuad&&) noexcept;
		DebugQuad& operator=(DebugQuad&&) noexcept;
	};

	/// @brief Meant to be used internally by the engine only.
	class HXG_DECLSPEC DebugRectangle : public virtual DebugQuad, public virtual Rectangle
	{
	public:

		DebugRectangle(const Vec2<int>& pos, const Vec2<int>& size, bool centered = true, Shader* shader = &line_shader);

		DebugRectangle() = default;

		DebugRectangle(DebugRectangle&&) noexcept;
		DebugRectangle& operator=(DebugRectangle&&) noexcept;
	};

	/// @brief Meant to be used internally by the engine only.
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