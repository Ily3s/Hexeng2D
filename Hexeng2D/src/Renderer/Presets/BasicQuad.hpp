#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include "../../Macros.hpp"
#include "../Mesh.hpp"
#include "InitPresets.hpp"

namespace Hexeng::Renderer::Presets
{

	HXG_DECLSPEC class BasicQuad : public Mesh
	{
	protected :

		static IndexBuffer s_index_buffer;

	public:

		Vec2<int> position{ 0, 0 };

		HXG_DECLSPEC static void init();

		HXG_DECLSPEC BasicQuad(const int* vertecies, Texture* texture, const Vec2<int>& pos = { 0, 0 }, Shader* shader = &basic_shader);

		HXG_DECLSPEC BasicQuad();

		HXG_DECLSPEC BasicQuad(BasicQuad&&) noexcept;
		HXG_DECLSPEC BasicQuad& operator=(BasicQuad&&) noexcept;

		static inline void stop() { s_index_buffer.~IndexBuffer(); }

	};

	HXG_DECLSPEC class BasicRectangle : public BasicQuad
	{
	public:

		HXG_DECLSPEC BasicRectangle(Vec2<int> pos, const Vec2<int>& size, Texture* texture, bool centered = false, Shader* shader = &basic_shader);
		HXG_DECLSPEC BasicRectangle(const Vec2<int>& pos, float size, Texture* texture, bool centerd = false, Shader* shader = &basic_shader);

		HXG_DECLSPEC BasicRectangle();

	};

	HXG_DECLSPEC class BasicSquare : public BasicRectangle
	{
	public:

		HXG_DECLSPEC BasicSquare(const Vec2<int>& pos, int size, Texture* texture, bool centered = false, Shader* shader = &basic_shader);
		HXG_DECLSPEC BasicSquare(const Vec2<int>& pos, float size, Texture* texture, bool centered = false, Shader* shader = &basic_shader);

		HXG_DECLSPEC BasicSquare();

	};

}

#endif // !BASIC_QUAD_HPP