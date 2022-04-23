#ifndef TEXT_HPP
#define TEXT_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "stb/stb_truetype.h"

#include "Texture.hpp"
#include "../Macros.hpp"
#include "Presets/BasicQuad.hpp"
#include "../Color.hpp"

namespace Hexeng::Renderer
{

	// optimizition todo : batch rendering

	struct CharBox
	{
		Vec2<float> min, max;
	};

	class HXG_DECLSPEC Font
	{
	public :

		std::unordered_map<char32_t, Texture> font_map;
		float quality = 0.1f;
		stbtt_fontinfo font_info;
		std::unique_ptr<uint8_t[]> file_buffer = nullptr;
		int line_height = 0, line_offset = 0;
		float get_advancement(int c);
		CharBox get_box(int c);

		// Warning : quality is recommanded to be within [0.01f, 1.0f]
		Font(const std::string& path, float quality = 0.1f);

		Font() = default;

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		Font(Font&&) noexcept = default;
		Font& operator=(Font&&) noexcept = default;

		void add_char(char32_t c);
	};

	enum class HorizontalAlign
	{
		LEFT,
		CENTER,
		RIGHT
	};

	enum class VerticalAlign
	{
		TOP,
		CENTER,
		BOT
	};

	class HXG_DECLSPEC Text
	{
	private :

		std::vector<Presets::BasicRectangle> m_chars;

	public :

		std::vector<Mesh*> chars;
		Color3 color;

		Text(std::u32string text, Font&, Vec2<int> position, int font_size,
			HorizontalAlign = HorizontalAlign::CENTER, VerticalAlign = VerticalAlign::CENTER, Color3 = Color3::white);

		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		Text(Text&&) noexcept = default;
		Text& operator=(Text&&) noexcept = default;
	};

}

#endif