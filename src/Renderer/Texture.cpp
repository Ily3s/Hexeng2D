#include "Texture.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	Texture::Texture(const std::string& filepath, unsigned int mag_filter, unsigned int min_filter)
		: m_filepath(filepath), m_mag_filter(mag_filter), m_min_filter(min_filter)
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* local_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_BPP, 4);

		HXG_SGL(glGenTextures(1, &m_id));
		HXG_SGL(glBindTexture(GL_TEXTURE_2D, m_id));

		HXG_SGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
		HXG_SGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
		HXG_SGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		HXG_SGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		HXG_SGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
		HXG_SGL(glBindTexture(GL_TEXTURE_2D, 0));

		if (local_buffer)
			stbi_image_free(local_buffer);
	}

	Texture::~Texture()
	{
		if (m_id)
		{
			HXG_SGL(glDeleteTextures(1, &m_id));
			m_id = 0;
		}
	}

	Texture::Texture(Texture&& other) noexcept
		: m_filepath(other.m_filepath), m_height(other.m_height), m_width(other.m_width), m_BPP(other.m_BPP),
		m_mag_filter(other.m_mag_filter), m_min_filter(other.m_min_filter), m_id(other.m_id)
	{
		other.m_id = 0;
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		m_filepath = other.m_filepath;
		m_BPP = other.m_BPP;
		m_height = other.m_height;
		m_id = other.m_id;
		m_mag_filter = other.m_mag_filter;
		m_min_filter = other.m_min_filter;
		m_width = other.m_width;
		other.m_id = 0;
		return *this;
	}

	void Texture::bind(unsigned int slot) const
	{
		HXG_SGL(glActiveTexture(slot));
		HXG_SGL(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture::unbind() const
	{
		HXG_SGL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::unload()
	{
		if (m_id)
		{
			HXG_SGL(glDeleteTextures(1, &m_id));
			m_id = 0;
		}
	}

	void Texture::load()
	{
		if (!m_id)
			*this = Texture(m_filepath, m_mag_filter, m_min_filter);
	}

}