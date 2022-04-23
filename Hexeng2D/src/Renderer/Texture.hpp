#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../Macros.hpp"
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "../Vectors.hpp"
#include <string>

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Texture
	{

	private:

		unsigned int m_id;
		std::string m_filepath;
		unsigned int m_mag_filter = GL_LINEAR, m_min_filter = GL_LINEAR;

		int m_width = 0, m_height = 0, m_BPP = 0;

		bool m_no_onload = false;

	public:

		Texture(const std::string& filepath, unsigned int mag_filter = GL_LINEAR, unsigned int min_filter = GL_LINEAR);
		Texture(const uint8_t* pixel_buffer, const Vec2<int>& size, GLuint sized_format = GL_RGBA8, GLuint base_format = GL_RGBA);

		~Texture();
		
		Texture() = default;
		
		void bind(unsigned int slot = GL_TEXTURE0) const;
		void unbind() const;
		 
		inline int get_width() const { return m_width; }
		inline int get_height() const { return m_height; }
		inline Vec2<int> get_size() const { return { m_width, m_height }; }
		inline unsigned int get_id() const { return m_id; }
		
		Vec2<int> size() const { return { m_width, m_height }; }
		 
		void unload();
		void load();
		
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		
		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;
	};

}

#endif // !TEXTURE_HPP