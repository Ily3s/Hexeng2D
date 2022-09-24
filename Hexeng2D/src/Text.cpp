#include <fstream>
#include <sstream>
#include <cassert>

#include "Text.hpp"
#include "Renderer/Renderer.hpp"

namespace Hexeng
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
		: color(c3), m_text(text), m_font(&font), m_pos(pos), m_font_size(font_size), m_ha(h_align), m_va(v_align)
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
 					const Renderer::TexCoord& coords = font.char_map[c];
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

		m_index_buffer = Renderer::IndexBuffer(raw_ib, GL_UNSIGNED_INT, ib_index);

		this->Mesh::Mesh(raw_vb, vb_index * 4, pos, Renderer::Quad::vertex_layout, &m_index_buffer, &font.texture, &Renderer::font_shader);

		uniforms.push_back({ &Renderer::u_color, &color });

		delete[] raw_vb;
		delete[] raw_ib;
	}

	std::vector<Text*> Text::s_translated_texts;

	Text::Text(const Language** language, std::u32string text, Font& font, Vec2<int> pos, int font_size, HorizontalAlign h_align, VerticalAlign v_align, Color3 c3)
		: Text((*language)->get_translation(text), font, pos, font_size, h_align, v_align)
	{
		m_language = language;
		m_text = text;
		s_translated_texts.push_back(this);
	}

	Text::Text(Text&& other) noexcept
		: Mesh(std::move(other)), m_text(std::move(other.m_text)), m_index_buffer(std::move(other.m_index_buffer)),
		m_font(other.m_font), m_pos(other.m_pos), m_font_size(other.m_font_size),
		m_ha(other.m_ha), m_va(other.m_va), m_language(other.m_language), color(other.color)
	{
		m_ib = &m_index_buffer;

		for (auto& [ui, value_ptr] : uniforms)
		{
			if (ui == &Renderer::u_color)
				value_ptr = &color;
		}

		if (m_language)
		{
			auto it = std::find(s_translated_texts.begin(), s_translated_texts.end(), &other);
			*it = this;
		}

		other.m_language = nullptr;
	}

	Text& Text::operator=(Text&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		m_index_buffer = std::move(other.m_index_buffer);
		m_ib = &m_index_buffer;
		m_text = std::move(other.m_text);
		m_font = other.m_font;
		m_pos = other.m_pos;
		m_font_size = other.m_font_size;
		m_ha = other.m_ha;
		m_va = other.m_va;
		color = other.color;

		for (auto& [ui, value_ptr] : uniforms)
		{
			if (ui == &Renderer::u_color)
				value_ptr = &color;
		}

		if (m_language)
		{
			auto it = std::find(s_translated_texts.begin(), s_translated_texts.end(), this);
			s_translated_texts.erase(it);
		}

		m_language = other.m_language;

		if (m_language)
		{
			auto it = std::find(s_translated_texts.begin(), s_translated_texts.end(), &other);
			*it = this;
		}

		other.m_language = nullptr;

		return *this;
	}

	void Text::reload_language()
	{
		Renderer::pending_actions.push_back([]() {
			for (Text* text : s_translated_texts)
			{
				*text = { text->m_language, text->m_text, *text->m_font, text->m_pos,
					text->m_font_size, text->m_ha, text->m_va, text->color };
			}
			});
	}

	std::unordered_map<std::u32string, size_t> Language::s_reference;

	static char32_t native_bom = U'\U0000FEFF';

	Language::Language(const std::string& filepath)
	{
		std::ifstream language_file(filepath, std::ios::binary);

		language_file.seekg(0, std::ios::end);
		size_t size = (size_t)language_file.tellg() - 4;
		language_file.seekg(0, std::ios::beg);

		char32_t bom = 0;
		language_file.read((char*)&bom, 4);

		char32_t* file_buffer = new char32_t[size / 4];
		language_file.read((char*)file_buffer, size);

		if (bom != native_bom)
		{
			char* endian_correct = new char[size];
			char* data = (char*)file_buffer;
			for (size_t i = 0; i < size; i+=4)
			{
				endian_correct[i] = data[i+3];
				endian_correct[i+1] = data[i+2];
				endian_correct[i+2] = data[i+1];
				endian_correct[i+3] = data[i];
			}

			delete[] file_buffer;
			file_buffer = (char32_t*)endian_correct;
		}

		size_t line_count = 0;
		size_t char_index = 0;

		for (bool keep_going = true; keep_going; line_count++)
		{
			std::u32string line;
			for (bool endl_reached = false; !endl_reached; char_index++)
			{
				if (char_index >= size / 4)
				{
					keep_going = false;
					break;
				}
				if (file_buffer[char_index] == U'\n')
					endl_reached = true;
				else
					line.push_back(file_buffer[char_index]);
			}
			m_language_table.insert({ line_count, line });
		}

		delete[] file_buffer;
	}

	void Language::set_reference_language(const Language* language)
	{
		s_reference.clear();
		for (auto& [i, str] : language->m_language_table)
			s_reference.insert({str, i});
	}

	const std::u32string& Language::get_translation(const std::u32string& input) const
	{
		return m_language_table.find(s_reference.find(input)->second)->second;
	}

}