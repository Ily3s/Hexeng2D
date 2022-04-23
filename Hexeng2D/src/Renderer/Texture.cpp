#include "Texture.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	Texture::Texture(const std::string& filepath, unsigned int mag_filter, unsigned int min_filter)
		: m_filepath(filepath), m_mag_filter(mag_filter), m_min_filter(min_filter)
	{
		stbi_set_flip_vertically_on_load(1);
		uint8_t* local_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_BPP, 4);

		HXG_GL(glGenTextures(1, &m_id));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, m_id));

		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		HXG_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, 0));

		if (local_buffer)
			stbi_image_free(local_buffer);

		ToBeDelete(this, [this]() { this->~Texture(); });
	}

	Texture::Texture(const uint8_t* pixel_buffer, const Vec2<int>& size, GLuint sized_format, GLuint base_format)
	{
		m_no_onload = true;
		m_width = size.x;
		m_height = size.y;

		HXG_GL(glGenTextures(1, &m_id));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, m_id));

		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		HXG_GL(glTexImage2D(GL_TEXTURE_2D, 0, sized_format, m_width, m_height, 0, base_format, GL_UNSIGNED_BYTE, pixel_buffer));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, 0));

		ToBeDelete(this, [this]() { this->~Texture(); });
	}

	Texture::~Texture()
	{
		if (m_id)
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	Texture::Texture(Texture&& other) noexcept
		: m_filepath(other.m_filepath), m_height(other.m_height), m_width(other.m_width), m_BPP(other.m_BPP),
		m_mag_filter(other.m_mag_filter), m_min_filter(other.m_min_filter), m_id(other.m_id), m_no_onload(other.m_no_onload)
	{
		other.m_id = 0;
		ToBeDelete(this, [this]() { this->~Texture(); });
		ToBeDelete::remove(&other);
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
		m_no_onload = other.m_no_onload;
		other.m_id = 0;

		ToBeDelete::remove(&other);
		ToBeDelete(this, [this]() { this->~Texture(); });

		return *this;
	}

	void Texture::bind(unsigned int slot) const
	{
		HXG_GL(glActiveTexture(slot));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture::unbind() const
	{
		HXG_GL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::unload()
	{
		if (m_id && !m_no_onload)
		{
			HXG_GL(glDeleteTextures(1, &m_id));
			m_id = 0;
		}
	}

	void Texture::load()
	{
		if (!m_id)
			*this = Texture(m_filepath, m_mag_filter, m_min_filter);
	}

}