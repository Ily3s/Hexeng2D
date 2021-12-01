#ifndef BASIC_QUAD_HPP
#define BASIC_QUAD_HPP

#include "../Mesh.hpp"
#include "InitPresets.hpp"

namespace Hexeng::Renderer::Presets
{

	class BasicQuad : public Mesh
	{
	protected :

		void verify_uniforms(Shader* shader);

	public:

		Vec2<int> position{ 0, 0 };

		static IndexBuffer s_index_buffer;
		static void init();

		BasicQuad(const int* vertecies, Texture* texture, const Vec2<int>& pos = { 0, 0 }, Shader* shader = &basic_shader);

		BasicQuad() = default;

		BasicQuad(BasicQuad&&) noexcept;
		BasicQuad& operator=(BasicQuad&&) noexcept;

		void draw() override;

	};

	class BasicRectangle : public BasicQuad
	{
	public:

		BasicRectangle(const Vec2<int>& pos, const Vec2<int>& size, Texture* texture, Shader* shader = &basic_shader);
		BasicRectangle(const Vec2<int>& pos, float size, Texture* texture, Shader* shader = &basic_shader);

		BasicRectangle() = default;

	};

	class BasicSquare : public BasicRectangle
	{
	private :

		static BasicRectangle s_basic_rec;
		static bool s_is_init;
		float m_size = 0.0f;

	public:

		BasicSquare(const Vec2<int>& pos, int size, Texture* texture, Shader* shader = &basic_shader);
		BasicSquare(const Vec2<int>& pos, float size, Texture* texture, Shader* shader = &basic_shader);

		BasicSquare() = default;

		void draw() final override;

	};

}

#endif // !BASIC_QUAD_HPP