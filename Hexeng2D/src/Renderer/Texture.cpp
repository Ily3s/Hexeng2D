#include <cassert>

#include "Texture.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

    Image::Image(const std::string& filepath, int channels_nb) : filepath(filepath)
    {
		stbi_set_flip_vertically_on_load(1);
        pixel_buffer = stbi_load(filepath.c_str(), &size.x, &size.y, &BPP, channels_nb);
    }

    Image::Image(Image&& other) noexcept
        : filepath(std::move(other.filepath)), size(other.size), pixel_buffer(other.pixel_buffer)
    {
        other.pixel_buffer = nullptr;
    }

    Image& Image::operator=(Image&& other) noexcept
    {
        filepath = std::move(other.filepath);
        size = other.size;
        if (pixel_buffer) stbi_image_free(pixel_buffer);
        pixel_buffer = other.pixel_buffer;
        other.pixel_buffer = nullptr;

        return *this;
    }

    Image::~Image() {
        if (pixel_buffer) stbi_image_free(pixel_buffer);
        pixel_buffer = nullptr;
    }

    uint8_t* Texture::to_pixel_buff(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(1);
		int four = 4;
		int sexteen = 16;
		return stbi_load(filepath.c_str(), &sexteen, &sexteen, &four, 4);
	}
	
    void Texture::free_pixel_buff(uint8_t* pb)
	{
		stbi_image_free(pb);
	}

	Texture::Texture(const std::string& filepath, const TexSettList& settings)
		: m_filepath(filepath)
	{
		m_build_settings(settings, {});

		stbi_set_flip_vertically_on_load(1);
		m_texbuffer = stbi_load(filepath.c_str(), &m_size.x, &m_size.y, &m_BPP, m_channels_nb);

		HXG_ASSERT(m_texbuffer,
			HXG_LOG_ERROR("Error when trying to load the texture \"" + filepath + "\" : " + stbi_failure_reason());
			return;);

		m_construct_texture();

		stbi_image_free(m_texbuffer);
		m_texbuffer = nullptr;

		ToBeDelete(this, [this]() { this->~Texture(); });
	}

	Texture::Texture(uint8_t*&& pixel_buffer, Vec2<int> size, const TexSettList& settings)
		: m_size(size),
		m_texbuffer(pixel_buffer)
	{
		pixel_buffer = nullptr;

		m_build_settings(settings, {});
		m_construct_texture();

		ToBeDelete(this, [this]() { this->~Texture(); });
	}

	void Texture::m_construct_texture()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, m_BPP);

		HXG_GL(glGenTextures(1, &m_id));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, m_id));

		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		HXG_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		HXG_GL(glTexImage2D(GL_TEXTURE_2D, 0, m_sized_format, m_size.x, m_size.y, 0, m_base_format, GL_UNSIGNED_BYTE, m_texbuffer));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::m_build_settings(const TexSettList& settings, std::vector<TexSett> necessary)
	{
		bool is_bpp_specified = false, is_channels_nb_specified = false;
		for (const auto& [setting, value] : settings)
		{
			auto it = std::find(necessary.begin(), necessary.end(), setting);
			if (it != necessary.end())
				necessary.erase(it);

			switch (setting)
			{
			case TexSett::MIN_FILTER: m_min_filter = value; break;
			case TexSett::MAG_FILTER: m_mag_filter = value; break;
            case TexSett::FILTER: m_mag_filter = m_min_filter = value; break;
			case TexSett::BASE_FORMAT: m_base_format = value; break;
			case TexSett::SIZED_FORMAT: m_sized_format = value; break;
			case TexSett::BPP: m_BPP = value; is_bpp_specified = true; break;
			case TexSett::CHANNELS_NB: m_channels_nb = value; is_channels_nb_specified = true; break;
			default: break;
			}
		}

		if (!is_bpp_specified)
		{
			switch (m_sized_format)
			{
			case GL_R8: m_BPP = 1; break;
			case GL_RGB8: m_BPP = 3; break;
			case GL_RGBA8: m_BPP = 4; break;
			case GL_RGBA16: m_BPP = 8; break;
			case GL_RGB16: m_BPP = 4; break;
			case GL_R16: m_BPP = 2; break;
				// maybe other internal formats todo
			default: break;
			}
		}

		if (!is_channels_nb_specified)
		{
			switch (m_sized_format)
			{
			case GL_R8: m_channels_nb = 1; break;
			case GL_RGB8: m_channels_nb = 3; break;
			case GL_RGBA8: m_channels_nb = 4; break;
			case GL_RGBA16: m_channels_nb = 4; break;
			case GL_RGB16: m_channels_nb = 3; break;
			case GL_R16: m_channels_nb = 1; break;
				// maybe other internal formats todo
			default: break;
			}
		}

		HXG_ASSERT(!necessary.size(),
			HXG_LOG_ERROR("Missing some necessary texture settings"););
	}

	Texture::~Texture()
	{
		if (m_id)
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;

			if (m_texbuffer)
				delete[] m_texbuffer;
		}
	}

	Texture::Texture(Texture&& other) noexcept
		: m_filepath(other.m_filepath),
		m_size(other.m_size),
		m_BPP(other.m_BPP),
		m_texbuffer(other.m_texbuffer),
		m_mag_filter(other.m_mag_filter),
		m_min_filter(other.m_min_filter),
		m_id(other.m_id),
		m_base_format(other.m_base_format),
		m_sized_format(other.m_sized_format),
		m_channels_nb(other.m_channels_nb)
	{
		other.m_id = 0;
		other.m_texbuffer = nullptr;
		ToBeDelete(this, [this]() { this->~Texture(); });
		ToBeDelete::remove(&other);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		m_filepath = other.m_filepath;
		m_texbuffer = other.m_texbuffer;
		m_BPP = other.m_BPP;
		m_size = other.m_size;
		m_id = other.m_id;
		m_mag_filter = other.m_mag_filter;
		m_min_filter = other.m_min_filter;
		m_base_format = other.m_base_format;
		m_sized_format = other.m_sized_format;
		m_channels_nb = other.m_channels_nb;
		other.m_id = 0;
		other.m_texbuffer = nullptr;

		ToBeDelete::remove(&other);
		ToBeDelete(this, [this]() { this->~Texture(); });

		return *this;
	}

	void Texture::bind(unsigned int slot) const
	{
		HXG_GL(glActiveTexture(slot));
		HXG_GL(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture::unbind()
	{
		HXG_GL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::unload()
	{
		if (m_id)
		{
			HXG_GL(glDeleteTextures(1, &m_id));
			m_id = 0;
		}
	}

	void Texture::load()
	{
		if (m_id)
			return;

		if (m_filepath.size())
		{
			*this = Texture(m_filepath, { {TexSett::MAG_FILTER, m_mag_filter}, {TexSett::MIN_FILTER, m_min_filter},
										{ TexSett::BASE_FORMAT, m_base_format}, {TexSett::SIZED_FORMAT, m_sized_format}, {TexSett::BPP, m_BPP} });
		}
		else
		{
			*this = Texture(std::move(m_texbuffer), m_size, { {TexSett::MAG_FILTER, m_mag_filter}, {TexSett::MIN_FILTER, m_min_filter},
										{ TexSett::BASE_FORMAT, m_base_format}, {TexSett::SIZED_FORMAT, m_sized_format}, {TexSett::BPP, m_BPP} });
		}
	}

	TextureVector::TextureVector(uint8_t*&& tex_buffer, Vec2<int> size, const TexSettList& settings)
		: Texture(std::move(tex_buffer), size, settings) {}

	TextureVector::TextureVector(const std::string& filepath, const TexSettList& settings)
		: Texture(filepath, settings) {}

	TextureVector::TextureVector(const TexSettList& settings)
	{
		m_build_settings(settings, {});
	}

	TextureVector::TextureVector(TextureVector&& other) noexcept
		: Texture(std::move(other)), m_coords(std::move(other.m_coords)) {}

	TextureVector& TextureVector::operator=(TextureVector&& other) noexcept
	{
		Texture::operator=(std::move(other));
		m_coords = std::move(other.m_coords);

		return *this;
	}

	void TextureVector::push(const uint8_t* tex_buffer, Vec2<int> size)
	{
		unload();

		Vec2<int> prev_size = { m_size.x * m_BPP, m_size.y };
		m_size.x += size.x;
		m_size.x *= m_BPP;
		m_size.y = std::max(m_size.y, size.y);

		uint8_t* new_texbuffer = new uint8_t[m_size.x * m_size.y];

		for (int row = 0; row < m_size.y; row++)
		{
			for (int col = 0; col < m_size.x; col++)
			{
				uint8_t& new_byte = new_texbuffer[row * m_size.x + col];
				if (col < prev_size.x && row < prev_size.y)
					new_byte = m_texbuffer[row * prev_size.x + col];
				else if (col >= prev_size.x && row < size.y)
					new_byte = tex_buffer[row * size.x + (col - prev_size.x)];
				else
					new_byte = 0;
			}
		}

		m_size.x /= m_BPP;
		if (m_texbuffer)
			delete[] m_texbuffer;
		m_texbuffer = new_texbuffer;

		load();

		m_coords.push_back({ {prev_size.x / m_BPP, 0},
							{m_size.x - 1, size.y - 1}});
	}

	void TextureVector::push(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(1);
		Vec2<int> new_texture_size;
		int bytes_per_pixel = 0;
		uint8_t* new_texture_buffer = stbi_load(filepath.c_str(), &new_texture_size.x, &new_texture_size.y, &bytes_per_pixel, 4);
		HXG_ASSERT(new_texture_buffer,
			HXG_LOG_ERROR("Error when trying to load the texture \"" + filepath + "\" : " + stbi_failure_reason());
			return;);
		HXG_ASSERT(bytes_per_pixel == m_BPP,
			HXG_LOG_ERROR("BPP not respected");
			stbi_image_free(new_texture_buffer); return;);
		push(new_texture_buffer, new_texture_size);
		stbi_image_free(new_texture_buffer);
	}

	const std::vector<iTexCoord>& TextureVector::get_coordinates() const
	{
		return m_coords;
	}

}
