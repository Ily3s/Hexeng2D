#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include <array>

#include "../Macros.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "DefaultShaders.hpp"

namespace Hexeng::Renderer
{

	/// @note Quad is in fact a rectangle, but it's shorter to say a quad.
	class HXG_DECLSPEC Quad : public virtual Mesh
	{
	public:

		/**
		* @param pos The position of the Quad.
		* @param size The dimentions (lenght, width) of the Quad.
		* @param centered If set to true, pos is the center of the quad, if set to false, pos is the bottom left of the quad.
		* @note If you want to make a custom shader for the quad,
		* take a look at DefaultShaders .glsl/.hpp/.cpp, copy paste basic_shader and modify it as you want.
		* */
		Quad(Vec2<float> pos, const Vec2<float>& size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);

		/// @brief The same as Quad(Vec2<float>, const Vec2<float>&, Texture*, bool, Shader*) except the size is a multiplier of the texture size.
		Quad(const Vec2<float>& pos, float size, Texture* texture, bool centerd = true, Shader* shader = &basic_shader);

		Quad() = default;
		~Quad() = default;

		Quad(Quad&&) noexcept;
		Quad& operator=(Quad&&) noexcept;

		inline static const VertexLayout& get_vertex_layout() { return s_vertex_layout; }

	private:

		Vec2<float> m_min{ 0, 0 }, m_max{ 0, 0 }, m_size{ 0, 0 };

	public:

		/// @brief Get the bottom left coordinates
		inline Vec2<float> get_min() { return m_min; }
		/// @brief Get the top right coordinates
		inline Vec2<float> get_max() { return m_max; }
		/// @brief get_max() - get_min()
		inline Vec2<float> get_size() { return m_size; }

	protected :

		static IndexBuffer s_index_buffer;
		static IndexBuffer s_edge_index_buffer;

		static ToBeInit s_init_components;

		static VertexLayout s_vertex_layout;
	};

	class HXG_DECLSPEC Square : public Quad
	{
	public:

		/// @brief The same as Quad(Vec2<float>, const Vec2<float>&, Texture*, bool, Shader*) except the size is the lenght of the square.
        /// @note This isn't the same Quad(..., float size, ...) where size is a multiplier of the texture size ! Here, size is the lenght of the square.
		Square(const Vec2<float>& pos, float size, Texture* texture, bool centered = true, Shader* shader = &basic_shader);

		Square() = default;
		~Square() = default;

		Square(Square&&) noexcept;
		Square& operator=(Square&&) noexcept;
	};

	/// @brief Meant to be used internally by the engine only.
	class HXG_DECLSPEC DebugQuad : public virtual Quad
	{
	public:

		DebugQuad(const Vec2<float>& pos, const Vec2<float>& size, bool centered = true, Shader* shader = &line_shader);

		DebugQuad() = default;

		DebugQuad(DebugQuad&&) noexcept;
		DebugQuad& operator=(DebugQuad&&) noexcept;
	};

	/// @brief Meant to be used internally by the engine only.
	class HXG_DECLSPEC DebugSquare : public virtual DebugQuad, public virtual Square
	{
	public:

		DebugSquare(const Vec2<float>& pos, float size, bool centered = false, Shader* shader = &line_shader);

		DebugSquare() = default;

		DebugSquare(DebugSquare&&) noexcept;
		DebugSquare& operator=(DebugSquare&&) noexcept;
	};

}

#endif // !BASIC_QUAD_HPP
