#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../Macros.hpp"
#include "glad/glad.h"
#include "stb/Image.h"
#include "../Vectors.hpp"
#include <string>

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Texture
	{

	private:

		unsigned int m_id;
		std::string m_filepath;
		unsigned int m_mag_filter, m_min_filter;

		int m_width = 0, m_height = 0, m_BPP = 0;

	public:

		Texture(const std::string& filepath, unsigned int mag_filter = GL_LINEAR, unsigned int min_filter = GL_LINEAR);
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