#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

#include "../Macros.hpp"
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "../Vectors.hpp"

namespace Hexeng::Renderer
{

	enum class TexSett
	{
		MAG_FILTER,
		MIN_FILTER,
		BASE_FORMAT,
		SIZED_FORMAT,
		BPP
	};

	using TexSettList = std::vector<std::pair<TexSett, GLuint>>;

	class HXG_DECLSPEC Texture
	{
	public :

		Texture(const std::string& filepath, const TexSettList& settings);
		Texture(uint8_t*&& pixel_buffer, Vec2<int> size, const TexSettList& settings);

		~Texture();
		
		Texture() = default;

		void bind(unsigned int slot = GL_TEXTURE0) const;
		static void unbind();
		 
		void unload();
		void load();
		
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		
		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;

	protected :

		GLuint m_id = 0;
		std::string m_filepath;
		uint8_t* m_texbuffer = nullptr;
		GLuint m_mag_filter = GL_LINEAR, m_min_filter = GL_LINEAR;
		GLuint m_base_format = GL_RGBA, m_sized_format = GL_RGBA8;
		Vec2<int> m_size{ 0, 0 };
		int m_BPP = 4;

		void build_settings(const TexSettList& settings, std::vector<TexSett> necessary);
		void construct_texture();

	public :

		inline int get_width() const { return m_size.x; }
		inline int get_height() const { return m_size.y; }
		inline Vec2<int> get_size() const { return m_size; }
		inline unsigned int get_id() const { return m_id; }
	};

	struct TexCoord
	{
		Vec2<int> bot_left, top_left, top_right, bot_right;
	};

	class HXG_DECLSPEC TextureVector : public Texture
	{
	public :

		TextureVector() = default;

		TextureVector(TextureVector&&) noexcept;
		TextureVector& operator=(TextureVector&&) noexcept;

		TextureVector(const std::string& filepath, const TexSettList& settings);
		TextureVector(uint8_t*&& tex_buffer, Vec2<int> size, const TexSettList& settings);
		TextureVector(const TexSettList& settings);

		void push(const uint8_t* tex_buffer, Vec2<int> size);

		const std::vector<TexCoord>& get_coordinates() const;

	private :

		std::vector<TexCoord> m_coords;
	};

}

#endif // !TEXTURE_HPP