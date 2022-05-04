#include <fstream>
#include <sstream>
#include <cassert>

#include "Text.hpp"
#include "Renderer.hpp"

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
		
		for (int row = 0; row < h; row++)
		{
			for (int col = 0; col < w; col++)
				bitmap_reversed[(h - row - 1) * w + col] = bitmap[row * w + col];
		}

		texture.push(bitmap_reversed, { w, h });
		char_map.insert({ c, texture.get_coordinates().back() });

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
			if (c != ' ' && c != '\n' && font.char_map.find(c) == font.char_map.end())
				font.add_char(c);
		}

		size_t countless_chars_nb = std::count(text.begin(), text.end(), ' ') + std::count(text.begin(), text.end(), '\n');

		float* raw_vb = new float[16 * (text.size() - countless_chars_nb)];
		uint32_t* raw_ib = new uint32_t[6 * (text.size() - countless_chars_nb)];

		float size = (float)font_size / font.line_height;

		std::vector<std::u32string> lines;

		std::u32string::iterator first_it = text.begin(), second_it = first_it;
		while (second_it != text.end())
		{
			second_it = std::find(first_it + 1, text.end(), '\n');
			lines.emplace_back(first_it, second_it);
			first_it = second_it;
		}

		Vec2<int> relative_pos = { 0, 0 };

		uint32_t vb_index = 0, ib_index = 0, vertecies_nb = 0;
		for (const std::u32string& line : lines)
		{
			Vec2<int> current_pos{ relative_pos };
			int width = 0;

			for (char32_t c : line)
			{
				if (c == '\n')
					continue;
				width += font.get_advancement(c) * size;
			}

			if (h_align == HorizontalAlign::CENTER)
				current_pos.x -= width / 2;
			else if (h_align == HorizontalAlign::RIGHT)
				current_pos.x -= width;

			if (v_align == VerticalAlign::CENTER)
				current_pos.y -= font_size / 2;
			else if (v_align == VerticalAlign::TOP)
				current_pos.y -= font_size;

			for (char32_t c : line)
			{
				if (c == '\n')
					continue;

				Vec2<float> box_min = font.get_box(c).min * size;
				current_pos += {(int)box_min.x, (int)box_min.y};

				if (c != ' ')
				{
 					const TexCoord& coords = font.char_map[c];
					Vec2<int> tex_size = coords.top_right - coords.bot_left;

					raw_vb[vb_index] = toX(current_pos.x); raw_vb[vb_index + 1] = toY(current_pos.y);
					raw_vb[vb_index + 4] = toX(current_pos.x); raw_vb[vb_index + 5] = toY(current_pos.y + tex_size.y * size);
					raw_vb[vb_index + 8] = toX(current_pos.x + tex_size.x * size); raw_vb[vb_index + 9] = toY(current_pos.y + tex_size.y * size);
					raw_vb[vb_index + 12] = toX(current_pos.x + tex_size.x * size); raw_vb[vb_index + 13] = toY(current_pos.y);

					raw_vb[vb_index + 2] = static_cast<float>(coords.bot_left.x); raw_vb[vb_index + 3] = static_cast<float>(coords.bot_left.y);
					raw_vb[vb_index + 6] = static_cast<float>(coords.top_left.x); raw_vb[vb_index + 7] = static_cast<float>(coords.top_left.y);
					raw_vb[vb_index + 10] = static_cast<float>(coords.top_right.x); raw_vb[vb_index + 11] = static_cast<float>(coords.top_right.y);
					raw_vb[vb_index + 14] = static_cast<float>(coords.bot_right.x); raw_vb[vb_index + 15] = static_cast<float>(coords.bot_right.y);

					raw_ib[ib_index] = vertecies_nb;
					raw_ib[ib_index + 1] = vertecies_nb + 1;
					raw_ib[ib_index + 2] = vertecies_nb + 2;

					raw_ib[ib_index + 3] = vertecies_nb + 2;
					raw_ib[ib_index + 4] = vertecies_nb + 3;
					raw_ib[ib_index + 5] = vertecies_nb;

					vb_index += 16;
					ib_index += 6;
					vertecies_nb += 4;
				}

				current_pos -= {(int)box_min.x, (int)box_min.y};
				current_pos.x += font.get_advancement(c) * size;
			}
		
			relative_pos.y -= font.line_offset * size;
		}

		m_index_buffer = IndexBuffer(raw_ib, GL_UNSIGNED_INT, ib_index);

		this->Mesh::Mesh(raw_vb, vb_index * 4, pos, Presets::basic_vertex_layout, &m_index_buffer, &font.texture, &Presets::font_shader);

		uniforms.push_back({ &Presets::u_color, &color });

		delete[] raw_vb;
		delete[] raw_ib;
	}

}