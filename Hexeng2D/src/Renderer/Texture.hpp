#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "../Macros.hpp"
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "../Vectors.hpp"

namespace Hexeng::Renderer
{

	/// <summary>
	/// An enum of possible texture settings you can specify to a texture constructor.
	/// </summary>
	enum class TexSett
	{
		/// Behaviour of a texture when it gets "zoomed in". By default, it's set to GL_LINEAR.
		MAG_FILTER,
		/// Behaviour of a texture when it gets "zoomed out". By default, it's set to GL_LINEAR.
		MIN_FILTER,
        /// Sets both MAG_FILTER and MIN_FILTER to the same value. By default, it's set to GL_LINEAR
        FILTER,
		/// The color format of the texture. By default, it's set to GL_RGBA.
		BASE_FORMAT,
		/// The color format with the size of each channel of the texture. By default, it's set to GL_RGBA8.
		SIZED_FORMAT,
		/// The number of bytes per pixel. By default, it is set to 4.
		BPP,
		/// The number of channels. By default, it is set to 4.
		CHANNELS_NB
	};

	using TexSettList = std::unordered_map<TexSett, GLuint>;

    class HXG_DECLSPEC Image
    {
    public:

        Image(const std::string& filepath, int channels_nb = 0);

        ~Image();

        Image() = default;

		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		Image(Image&&) noexcept;
		Image& operator=(Image&&) noexcept;

    public:

		std::string filepath;
		uint8_t* pixel_buffer = nullptr;
		Vec2<int> size{ 0, 0 };
		int BPP = 4;
    };

	class HXG_DECLSPEC Texture
	{
	public:

		/// @brief construct from an image file.
		/// <param name="filepath">The file path to an image.</param>
		/// <param name="settings">A map of {TexSett, (unsigned int) value}. Take a look at TexSett for more infos.</param>
		Texture(const std::string& filepath, const TexSettList& settings = {});

		/// @brief construct from a pixel buffer.
		/// <param name="pixel_buffer">An r-value reference to the heap allocated pixel buffer.
		/// Deallocate becomes the responsability of the engine.</param>
		/// <param name="size">The dimentions (lenght, width) of the pixel buffer.</param>
		/// <param name="settings">A map of {TexSett, (unsigned int) value}. Take a look at TexSett for more infos.</param>
		Texture(uint8_t*&& pixel_buffer, Vec2<int> size, const TexSettList& settings = {});

		~Texture();

		Texture() = default;

		void bind(unsigned int slot = GL_TEXTURE0) const;
		static void unbind();

		/// <summary>
		/// Unloads the texture from the vram.
		/// In sight to load again, either the file path or the pixel buffer stays in the ram.
		/// </summary>
		void unload();

		/// <summary>
		/// Reconstruct the texture the same way it has been constructed.
		/// Either with the file path or with the pixel buffer.
		/// </summary>
		void load();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;

		static uint8_t* to_pixel_buff(const std::string& filepath);
		static void free_pixel_buff(uint8_t*);

	protected:

		GLuint m_id = 0;
		std::string m_filepath;
		uint8_t* m_texbuffer = nullptr;
		GLuint m_mag_filter = GL_LINEAR, m_min_filter = GL_LINEAR;
		GLuint m_base_format = GL_RGBA, m_sized_format = GL_RGBA8;
		Vec2<int> m_size{ 0, 0 };
		int m_BPP = 4, m_channels_nb = 4;

		void m_build_settings(const TexSettList& settings, std::vector<TexSett> necessary);
		void m_construct_texture();

	public:

		inline int get_width() const { return m_size.x; }
		inline int get_height() const { return m_size.y; }
		inline Vec2<int> get_size() const { return m_size; }
		inline unsigned int get_id() const { return m_id; }
	};

	struct iTexCoord
	{
		Vec2<int> bot_left, top_right;
	};

	struct fTexCoord
	{
		Vec2<float> bot_left, top_right;
	};

    // todo : TextureVector::reserve, operator[] ...

	/// <summary>
	/// Allows to push multiple textures linearly to form a unique texture that is in fact a one dimentional array of textures.
	/// </summary>
	/// @note If the different textures are not the same size, this may be unoptimized for memory.
	/// Also if you're looking for a texture for batch rendering or similar, you should use TextureAtlas instead.
	class HXG_DECLSPEC TextureVector : public Texture
	{
	public:

		TextureVector() = default;

		TextureVector(TextureVector&&) noexcept;
		TextureVector& operator=(TextureVector&&) noexcept;

		/// @brief Same as Texture::Texture(const std::string& filepath, const TexSettList& settings = {})
		TextureVector(const std::string& filepath, const TexSettList& settings = {});

		/// @brief Same as Texture::Texture(uint8_t*&& pixel_buffer, Vec2<int> size, const TexSettList& settings = {})
		TextureVector(uint8_t*&& tex_buffer, Vec2<int> size, const TexSettList& settings = {});

		/// @brief Only specify settings for the moment.
		/// <param name="settings">A map of {TexSett, (unsigned int) value}. Take a look at TexSett for more infos.</param>
		TextureVector(const TexSettList& settings);

		/// @brief Push back a texture to the array using a pixel buffer
		/// @param tex_buffer The pixel buffer.
		/// Note that, unlike with constructors, you still have responsability of the buffer after the call of this function.
		/// @param size The size of the the dimentions (lenght, width) of the texture buffer.
		/// @note The color format, sized format, bytes per pixel and channels number of the texture buffer
		/// have to respect what has been specified in TexSettList during construction of this TextureVector
		/// or what's by default (GL_RGBA, GL_RGBA8, 4, 4).
		void push(const uint8_t* tex_buffer, Vec2<int> size);

		/// @brief Push back a texture to the array using the filepath to an image
		/// @param filepath The filepath to the image.
		/// @note The color format, sized format, bytes per pixel and channels number of the image file
		/// have to respect what has been specified in TexSettList during construction of this TextureVector
		/// or what's by default (GL_RGBA, GL_RGBA8, 4, 4).
		void push(const std::string& filepath);

		/// @brief The coordinates of the textures you have pushed to the vector.
		const std::vector<iTexCoord>& get_coordinates() const;

	private:

		std::vector<iTexCoord> m_coords;
	};

}

#endif // !TEXTURE_HPP
