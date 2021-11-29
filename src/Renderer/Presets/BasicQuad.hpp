#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include "../Mesh.hpp"
#include "InitPresets.hpp"

namespace Hexeng::Renderer::Presets
{

	class BasicQuad : public Mesh
	{
	public :

		static IndexBuffer s_index_buffer;
		static void init();

		BasicQuad(const int* positions, Texture* texture, Shader* shader = &basic_shader);

		BasicQuad() = default;

		void draw() override;

	};

	class BasicRectangle : public BasicQuad
	{
	public :

		BasicRectangle(const Vec2<int>& origin, Vec2<int> size, Texture* texture, Shader* shader = &basic_shader);
		BasicRectangle(const Vec2<int>& origin, float size, Texture* texture, Shader* shader = &basic_shader);

		BasicRectangle() = default;

	};

	class BasicSquare : public BasicRectangle
	{
	public :

		BasicSquare(const Vec2<int>& origin, int size, Texture* texture, Shader* shader = &basic_shader);
		BasicSquare(const Vec2<int>& origin, float size, Texture* texture, Shader* shader = &basic_shader);

		BasicSquare() = default;

	};

}

#endif // !BASIC_QUAD_HPP