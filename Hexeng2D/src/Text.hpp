#ifndef TEXT_HPP
#define TEXT_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "stb/stb_truetype.h"

#include "Renderer/Texture.hpp"
#include "Macros.hpp"
#include "Renderer/Quad.hpp"
#include "Color.hpp"

namespace Hexeng
{

	// optimizition todo : batch rendering

	struct CharBox
	{
		Vec2<float> min, max;
	};

	class HXG_DECLSPEC Font
	{
	public :

		Renderer::TextureVector texture{ {{Renderer::TexSett::BASE_FORMAT, GL_RED}, {Renderer::TexSett::SIZED_FORMAT, GL_R8}} };
		std::unordered_map<char32_t, Renderer::TexCoord> char_map;
		float quality = 0.1f;
		stbtt_fontinfo font_info{};
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

	class HXG_DECLSPEC Language
	{
	public :

		Language(const std::string& filepath);

		static void set_reference_language(const Language* language);

		const std::u32string& get_translation(const std::u32string& input) const;

	private :

		static std::unordered_map<std::u32string, size_t> s_reference;

		std::unordered_map<size_t, std::u32string> m_language_table;
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

	class HXG_DECLSPEC Text : public Renderer::Mesh
	{
	public :

		Color3 color;

		Text(std::u32string text, Font&, Vec2<int> position, int font_size,
			HorizontalAlign = HorizontalAlign::CENTER, VerticalAlign = VerticalAlign::CENTER, Color3 = Color3::white);

		Text(const Language** language, std::u32string text, Font&, Vec2<int> position, int font_size,
			HorizontalAlign = HorizontalAlign::CENTER, VerticalAlign = VerticalAlign::CENTER, Color3 = Color3::white);

		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		Text(Text&&) noexcept;
		Text& operator=(Text&&) noexcept;

		static void reload_language();

	private :

		Renderer::IndexBuffer m_index_buffer;

		std::u32string m_text;
		Font* m_font;
		Vec2<int> m_pos;
		int m_font_size;
		HorizontalAlign m_ha;
		VerticalAlign m_va;
		const Language** m_language = nullptr;

		static std::vector<Text*> s_translated_texts;
	};

}

#endif