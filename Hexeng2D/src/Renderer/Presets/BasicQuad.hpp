#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include "../../Macros.hpp"
#include "../Mesh.hpp"
#include "InitPresets.hpp"

namespace Hexeng::Renderer::Presets
{

	class HXG_DECLSPEC BasicQuad : public Mesh
	{
	protected:

		static IndexBuffer s_index_buffer;
		static IndexBuffer s_edge_index_buffer;

	public:

		Vec2<int> position{ 0, 0 };

		static void init();

		BasicQuad(const int* vertecies, Texture* texture, const Vec2<int>& pos = { 0, 0 }, Shader* shader = &basic_shader);

		BasicQuad() = default;

		BasicQuad(BasicQuad&&) noexcept;
		BasicQuad& operator=(BasicQuad&&) noexcept;

		~BasicQuad() = default;

		static inline void stop() { s_index_buffer.~IndexBuffer(); s_edge_index_buffer.~IndexBuffer(); }

	};

	class HXG_DECLSPEC BasicRectangle : public virtual BasicQuad
	{
	public:

		BasicRectangle(Vec2<int> pos, const Vec2<int>& size, Texture* texture, bool centered = false, Shader* shader = &basic_shader);
		BasicRectangle(const Vec2<int>& pos, float size, Texture* texture, bool centerd = false, Shader* shader = &basic_shader);

		BasicRectangle() = default;
		~BasicRectangle() = default;

		BasicRectangle(BasicRectangle&&) noexcept;
		BasicRectangle& operator=(BasicRectangle&&) noexcept;
	};

	class HXG_DECLSPEC BasicSquare : public BasicRectangle
	{
	public:

		BasicSquare(const Vec2<int>& pos, int size, Texture* texture, bool centered = false, Shader* shader = &basic_shader);
		BasicSquare(const Vec2<int>& pos, float size, Texture* texture, bool centered = false, Shader* shader = &basic_shader);

		BasicSquare() = default;
		~BasicSquare() = default;

		BasicSquare(BasicSquare&&) noexcept;
		BasicSquare& operator=(BasicSquare&&) noexcept;
	};

	class HXG_DECLSPEC DebugQuad : public virtual BasicQuad
	{
	public:

		DebugQuad(const int* vertecies, const Vec2<int>& pos = { 0, 0 }, Shader* shader = &line_shader);

		DebugQuad() = default;

		DebugQuad(DebugQuad&&) noexcept;
		DebugQuad& operator=(DebugQuad&&) noexcept;
	};

	class HXG_DECLSPEC DebugRectangle : public virtual DebugQuad, public virtual BasicRectangle
	{
	public:

		DebugRectangle(const Vec2<int>& pos, const Vec2<int>& size, bool centered = false, Shader* shader = &line_shader);

		DebugRectangle() = default;

		DebugRectangle(DebugRectangle&&) noexcept;
		DebugRectangle& operator=(DebugRectangle&&) noexcept;
	};

	class HXG_DECLSPEC DebugSquare : public virtual DebugRectangle, public virtual BasicSquare
	{
	public:

		DebugSquare(const Vec2<int>& pos, int size, bool centered = false, Shader* shader = &line_shader);

		DebugSquare() = default;

		DebugSquare(DebugSquare&&) noexcept;
		DebugSquare& operator=(DebugSquare&&) noexcept;
	};

}

#endif // !BASIC_QUAD_HPP