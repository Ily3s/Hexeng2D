#ifndef UTILS_HPP
#define UTILS_HPP

#include "Text.hpp"
#include "Renderer/Quad.hpp"
#include "EventManager/Button.hpp"

class DefaultButton
{
public :

	DefaultButton(const std::u32string& str, std::function<void(void)> on_click, Hexeng::Vec2<float> pos,
		float size, Hexeng::Color4 behind_color, Hexeng::Color4 color = Hexeng::Color4::white, Hexeng::Range range = Hexeng::Range::LOCAL);

	DefaultButton() = default;

	DefaultButton(const DefaultButton&) = delete;
	DefaultButton& operator=(const DefaultButton&) = delete;

	DefaultButton(DefaultButton&&) noexcept;
	DefaultButton& operator=(DefaultButton&&) noexcept;

private:

	std::function<void(void)> m_on_click = []() {};
	Hexeng::Vec2<float> m_pos{ 0, 0 }, m_size{ 0, 0 };
	Hexeng::Range m_range = Hexeng::Range::LOCAL;

public :

	Hexeng::Renderer::Quad frame;
	Hexeng::Text text;
	Hexeng::Renderer::Quad invert_quad;
	Hexeng::Renderer::SuperMesh mesh;

	Hexeng::EventManager::Button button;

	void enable();
	void disable();
	void switch_on_off();

};

class Cursor
{
public :

	Cursor(const std::u32string& str, float* var, Hexeng::Vec2<float> pos, Hexeng::Vec2<float> size,
		Hexeng::Color4 color = Hexeng::Color4::white, Hexeng::Range range = Hexeng::Range::LOCAL);

	Cursor() = default;

	Cursor(const Cursor&) = delete;
	Cursor& operator=(const Cursor&) = delete;

	Cursor(Cursor&&) noexcept;
	Cursor& operator=(Cursor&&) noexcept;

	float* var = nullptr;

	Hexeng::Text text;
	Hexeng::Renderer::Quad stick;
	Hexeng::Renderer::Square quad;
	Hexeng::Renderer::SuperMesh mesh;

	Hexeng::EventManager::Button evt;

	void enable();
	void disable();
	void switch_on_off();
};

#endif