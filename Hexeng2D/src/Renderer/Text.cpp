#include <fstream>
#include <sstream>
#include <cassert>

#include "Text.hpp"

namespace Hexeng::Renderer
{

	Font::Font(const std::string& path, float quality) : quality(quality)
	{
		std::basic_ifstream<unsigned char> font_file{ path, std::ios::in | std::ios::binary };

		assert(font_file && "This file may not exists");

		font_file.seekg(0, std::ios::end);
		size_t size = font_file.tellg();
		font_file.seekg(0);

		file_buffer = std::make_unique<uint8_t[]>(size);
		font_file.read(file_buffer.get(), size);

		stbtt_InitFont(&font_info, file_buffer.get(), stbtt_GetFontOffsetForIndex(file_buffer.get(), 0));

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &line_gap);
		line_height = (ascent - descent) * quality;
		line_offset = line_gap * quality + line_height;
	}

	void Font::add_char(char32_t c)
	{
		int w, h;

		uint8_t* bitmap = stbtt_GetCodepointBitmap(&font_info, 0, quality, c, &w, &h, 0, 0);

		uint8_t* bitmap_reversed = new uint8_t[w * h];
		
		for (int row = 0; row < h; ++row)
		{
			for (int col = 0; col < w; ++col)
				bitmap_reversed[(h - row - 1) * w + col] = bitmap[row * w + col];
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		font_map.insert({ (char32_t)c, Texture{bitmap_reversed, {w, h}, GL_R8, GL_RED} });

		stbtt_FreeBitmap(bitmap, font_info.userdata);
		delete[] bitmap_reversed;
	}

	float Font::get_advancement(int c)
	{
		int adv;
		stbtt_GetCodepointHMetrics(&font_info, c, &adv, 0);
		return (float)adv * quality;
	}

	CharBox Font::get_box(int c)
	{
		int x0, x1, y0, y1;
		stbtt_GetGlyphBox(&font_info, stbtt_FindGlyphIndex(&font_info, c), &x0, &y0, &x1, &y1);
		return { {(float)x0 * quality, (float)y0 * quality}, {(float)x1 * quality, (float)y1 * quality} };
	}

	Text::Text(std::u32string text, Font& font, Vec2<int> pos, int font_size, HorizontalAlign h_align, VerticalAlign v_align, Color3 c3)
		: color(c3)
	{
		for (char32_t c : text)
		{
			if (font.font_map.find(c) == font.font_map.end())
				font.add_char(c);
		}

		float size = (float)font_size / font.line_height;

		std::vector<std::u32string> lines;

		std::u32string::iterator first_it = text.begin(), second_it = first_it;
		while (second_it != text.end())
		{
			second_it = std::find(first_it + 1, text.end(), '\n');
			lines.emplace_back(first_it, second_it);
			first_it = second_it;
		}

		for (const std::u32string& line : lines)
		{
			Vec2<int> cursor_pos{ pos };
			int width = 0;

			for (char32_t c : line)
			{
				if (c == '\n')
					continue;
				width += font.get_advancement(c) * size;
			}

			if (h_align == HorizontalAlign::CENTER)
				cursor_pos.x -= width / 2;
			else if (h_align == HorizontalAlign::RIGHT)
				cursor_pos.x -= width;

			if (v_align == VerticalAlign::CENTER)
				cursor_pos.y -= font_size / 2;
			else if (v_align == VerticalAlign::TOP)
				cursor_pos.y -= font_size;

			for (char32_t c : line)
			{
				if (c == '\n')
					continue;
				Vec2<float> box_min = font.get_box(c).min * size;
				cursor_pos += {(int)box_min.x, (int)box_min.y};
				m_chars.push_back(std::move(Presets::BasicRectangle(cursor_pos, size, &font.font_map[c], false, &Presets::font_shader)));
				m_chars.back().uniforms.push_back({ &Presets::u_color, &color });
				int advancement;
				stbtt_GetCodepointHMetrics(&font.font_info, c, &advancement, 0);
				cursor_pos -= {(int)box_min.x, (int)box_min.y};
				cursor_pos.x += font.get_advancement(c) * size;
			}

			pos.y -= font.line_offset * size;
		}

		for (auto& mesh : m_chars)
			chars.push_back(&mesh);
	}

}