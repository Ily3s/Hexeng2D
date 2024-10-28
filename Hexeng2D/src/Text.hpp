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

    // todo : UTF-32 ???
    // todo : dynamic quality

	struct CharBox
	{
		Vec2<float> min, max;
	};

	/// <summary>
	/// Loads a font from a ttf file that will be used when writing Text.
	/// </summary>
	/// @note Only the constructor should be used by the user.
	class HXG_DECLSPEC Font
	{
	public :

		Renderer::TextureVector texture{ {{Renderer::TexSett::BASE_FORMAT, GL_RED}, {Renderer::TexSett::SIZED_FORMAT, GL_R8}} };
		std::unordered_map<char32_t, Renderer::iTexCoord> char_map;
		float quality = 0.1f;
		stbtt_fontinfo font_info{};
		std::vector<uint8_t> file_buffer;
		int line_height = 0, line_offset = 0;
		float get_advancement(int c);
		CharBox get_box(int c);

		/// @note Quality is recommanded to be within [0.01f, 1.0f]
		Font(const std::string& path, float quality = 0.1f);

		Font() = default;

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		Font(Font&&) noexcept = default;
		Font& operator=(Font&&) noexcept = default;

		void add_char(char32_t c);

	private :

		std::string m_filepath;
	};

	/// <summary>
	/// Loads a dictionary from a language file.
	/// </summary>
	/// @details The language file should be written in UTF-32 (any endian) with BOM.
	/// You can write that type of file with EditPad Lite for example.
	/// Also the line breaks should be LF and not CRLF (If you are on windows, you may need to set line break style to LF).
	/// The translation of the line n in the language file A should be written in all langage files at line n.
	class HXG_DECLSPEC Language
	{
	public :

		Language(const std::string& filepath);

		/// <summary>
		/// Set a reference language that will be used when writing text.
		/// </summary>
		/// <param name="language">A pointer to the language that will be used as a reference.</param>
		/// @note This function has to be called in order to use the Language System.
		static void set_reference_language(const Language* language);

		/// <summary>
		/// Get the translation of input
		/// </summary>
		/// <param name="input">A sentence in the reference language</param>
		/// <returns>The translation of input in this language</returns>
		/// @note This function is meant to be used internally by the engine.
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

		Text() = default;

		/// <param name="text">The text to be written. Format : UTF-32</param>
		/// <param name="font">The Font that will be used to texture the Mesh</param>
		/// <param name="position">The absolute position of a certain point of the Text.
		/// The relative position of this point to the Text is defined by va and ha</param>
		/// <param name="font_size">A size in the Hexeng2D coordinate system</param>
		/// <param name="ha">Defines the x position of the text relative to position</param>
		/// <param name="va">Defines the y position of the text relative to position</param>
		/// <param name="c">The color of the text</param>
		/// <param name="box">A box that must contain the text</param>
		/// <param name="use_what_doesnt_fit">A function that uses what doesn't fit in the box</param>
		
		Text(std::u32string text, Font& font, Vec2<float> position, int font_size,
			HorizontalAlign ha= HorizontalAlign::CENTER, VerticalAlign va= VerticalAlign::CENTER, Color4 c= Color4::white,
			Vec2<float> box = {0, 0}, std::function<void(const std::u32string&)> use_what_doesnt_fit = nullptr);

		/// <param name="language">A pointer to the current language's pointer.
		/// It is a pointer to a pointer so that it is easier to switch languages</param>
		/// <param name="text">The text to be written. Format : UTF-32</param>
		/// <param name="font">The Font that will be used to texture the Mesh</param>
		/// <param name="position">The absolute position of a certain point of the Text.
		/// The relative position of this point to the Text is defined by va and ha</param>
		/// <param name="font_size">A size in the Hexeng2D coordinate system</param>
		/// <param name="ha">Defines the x position of the text relative to position</param>
		/// <param name="va">Defines the y position of the text relative to position</param>
		/// <param name="c">The color of the text</param>
		/// <param name="box">A box that must contain the text</param>
		/// <param name="use_what_doesnt_fit">A function that uses what doesn't fit in the box</param>
		
		Text(const Language** language, std::u32string text, Font& font, Vec2<float> position, int font_size,
			HorizontalAlign ha = HorizontalAlign::CENTER, VerticalAlign va = VerticalAlign::CENTER, Color4 c= Color4::white,
			Vec2<float> box = {0, 0}, std::function<void(const std::u32string&)> use_what_doesnt_fit = nullptr);

		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		Text(Text&&) noexcept;
		Text& operator=(Text&&) noexcept;

		/// <summary>
		/// Refresh all the texts with the new language.
		/// </summary>
		/// @details After switching languages, this function has to be called to refresh the texts.
		static void reload_language();

		/// <returns>The size of characters being rendered (spaces don't count)</returns>
		size_t get_char_count();

		/// <summary>
		/// Set a list of character that needs to be rendered
		/// </summary>
		/// <param name="enabled_chars">A list of bools that indicates if a character have to be rendered.
		/// Its size has to be equal to get_char_count() </param>
		void set_enabled_chars(std::vector<bool> enabled_chars);

	private :

		Renderer::IndexBuffer m_index_buffer;

		std::u32string m_text;
		Font* m_font;
		Vec2<float> m_pos;
		int m_font_size;
		HorizontalAlign m_ha;
		VerticalAlign m_va;
		const Language** m_language = nullptr;

		static std::vector<Text*> s_translated_texts;

		std::vector<uint32_t> m_raw_ib;

		Vec2<float> m_box = { 0, 0 };
		std::function<void(const std::u32string&)> m_use_extra = nullptr;
	};

}

#endif
