#include "Utils.hpp"
#include "Resources.hpp"

DefaultButton::DefaultButton(const std::u32string& str, std::function<void(void)> on_click, Hexeng::Vec2<float> pos,
	float size, Hexeng::Color4 behind_color_p, Hexeng::Color4 color, Hexeng::Range range)
	: mesh({ &frame, &invert_quad, &text }),
	frame(pos, { Hexeng::Vec2<float>{ 54, 9 } * size }, nullptr),
	text(&language_switch, str, roboto_font, { pos.x, pos.y - (2 * size) }, 7 * size),
	invert_quad(pos, Hexeng::Vec2<float>{52, 7} *size, nullptr),
	button(pos - Hexeng::Vec2<float>{54, 9} * (size / 2.0f), pos + Hexeng::Vec2<float>{54, 9} * (size / 2.0f),
		{
			{Hexeng::EventManager::ButtonEvent::CLICK, [this]() { button_click_snd.play();  m_on_click(); }},
			{Hexeng::EventManager::ButtonEvent::HOVER, [this]()
				{button_hover_snd.play(); 
				frame.opacity = 0;
				text.color_filter = invert_quad.color;
				invert_quad.color = frame.color; }},
			{Hexeng::EventManager::ButtonEvent::UNHOVER, [this]()
				{frame.opacity = 1;
				invert_quad.color = text.color_filter;
				text.color_filter = frame.color_filter; } }
		}, GLFW_MOUSE_BUTTON_1, m_range),
	m_on_click(on_click), m_pos(pos), m_size(size), m_range(range)
{
	text.color_filter = color;
	invert_quad.color = behind_color_p;
	frame.color = color;
}

DefaultButton::DefaultButton(DefaultButton&& other) noexcept
	: frame(std::move(other.frame)), text(std::move(other.text)), invert_quad(std::move(other.invert_quad)),
	mesh({&frame, &invert_quad, &text }),
	m_pos(other.m_pos), m_size(other.m_size), m_range(other.m_range), m_on_click(std::move(other.m_on_click)),
	button(std::move(other.button))
{
	button.click = [this]() { button_click_snd.play();  m_on_click(); };
	button.hover = [this]()
	{button_hover_snd.play(); 
	frame.opacity = 0;
	text.color_filter = invert_quad.color;
	invert_quad.color = frame.color; };
	button.unhover = [this]()
	{frame.opacity = 1;
	invert_quad.color = text.color_filter;
	text.color_filter = frame.color_filter; };
}

DefaultButton& DefaultButton::operator=(DefaultButton&& other) noexcept
{
	frame = std::move(other.frame);
	text = std::move(other.text);
	invert_quad = std::move(other.invert_quad);
	mesh = { {&frame, &invert_quad, &text} };

	m_on_click = std::move(other.m_on_click);
	m_size = other.m_size;
	m_pos = other.m_pos;
	m_range = other.m_range;

	button = std::move(other.button);

	button.click = [this]() { button_click_snd.play();  m_on_click(); };
	button.hover = [this]()
	{button_hover_snd.play(); 
	frame.opacity = 0;
	text.color_filter = invert_quad.color;
	invert_quad.color = frame.color; };
	button.unhover = [this]()
	{frame.opacity = 1;
	invert_quad.color = text.color_filter;
	text.color_filter = frame.color_filter; };

	return *this;
}

void DefaultButton::enable()
{
	mesh.enable = true;
	button.enable = true;
}

void DefaultButton::disable()
{
	mesh.enable = false;
	button.enable = false;
}

void DefaultButton::switch_on_off()
{
	mesh.enable = !mesh.enable;
	button.enable = !button.enable;
}

Cursor::Cursor(const std::u32string& str, float* var_p, Hexeng::Vec2<float> pos, Hexeng::Vec2<float> size,
	Hexeng::Color4 color, Hexeng::Range range)
	: mesh({ &stick, &text, &quad }), var(var_p),
	text(&language_switch, str, roboto_font, { pos.x - (4.0f / 10.0f * size.x),
		pos.y + (5.0f / 8.0f * size.y) }, (1.0f / 2.0f * size.y)),
	quad({ stick.get_min().x + ((*var) * stick.get_size().x), pos.y }, size.y, nullptr),
	stick(pos, { size.x, (1.0f / 3.0f * size.y) }, nullptr),
	evt({ pos - Hexeng::Vec2<float>{size.x + size.y, size.y} / 2 }, { pos + Hexeng::Vec2<float>{size.x + size.y, size.y} / 2 },
		{
			{Hexeng::EventManager::ButtonEvent::KEEP_CLICKING, [this]()
				{ if ((Hexeng::EventManager::mouse_position.x < (evt.max.x - (evt.max.y - evt.min.y)/2)) &&
					(Hexeng::EventManager::mouse_position.x > (evt.min.x + (evt.max.y - evt.min.y) / 2))) {
			quad.position.x = Hexeng::EventManager::mouse_position.x;
				*var = static_cast<float>(quad.position.x - stick.get_min().x) / stick.get_size().x;} } }
		}, GLFW_MOUSE_BUTTON_1, range)
{
	quad.color = color;
	stick.color = color;
	text.color_filter = color;
}

Cursor::Cursor(Cursor&& other) noexcept
	: mesh({ &stick, &text, &quad }), var(other.var),
	text(std::move(other.text)), quad(std::move(other.quad)), stick(std::move(other.stick)),
	evt(std::move(other.evt))
{
	evt.keep_clicking = [this]()
	{ if ((Hexeng::EventManager::mouse_position.x < (evt.max.x - (evt.max.y - evt.min.y) / 2)) &&
		(Hexeng::EventManager::mouse_position.x > (evt.min.x + (evt.max.y - evt.min.y) / 2))) {
		quad.position.x = Hexeng::EventManager::mouse_position.x;
		*var = static_cast<float>(quad.position.x - stick.get_min().x) / stick.get_size().x;
	} };
}

Cursor& Cursor::operator=(Cursor&& other) noexcept
{
	var = other.var;

	text = std::move(other.text);
	quad = std::move(other.quad);
	stick = std::move(other.stick);
	mesh = { { &stick, &text, &quad } };

	evt = std::move(other.evt);

	evt.keep_clicking = [this]()
	{ if ((Hexeng::EventManager::mouse_position.x < (evt.max.x - (evt.max.y - evt.min.y) / 2)) &&
		(Hexeng::EventManager::mouse_position.x > (evt.min.x + (evt.max.y - evt.min.y) / 2))) {
		quad.position.x = Hexeng::EventManager::mouse_position.x;
		*var = static_cast<float>(quad.position.x - stick.get_min().x) / stick.get_size().x;
	} };

	return *this;
}

void Cursor::enable()
{
	mesh.enable = true;
	evt.enable = true;
}

void Cursor::disable()
{
	mesh.enable = false;
	evt.enable = false;
}

void Cursor::switch_on_off()
{
	mesh.enable = !mesh.enable;
	evt.enable = !evt.enable;
}