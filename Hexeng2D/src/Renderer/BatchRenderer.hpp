#ifndef BATCH_RENDERER_HPP
#define BATCH_RENDERER_HPP

#include "Texture.hpp"
#include "Mesh.hpp"
#include "DefaultShaders.hpp"

#include <unordered_map>
#include <vector>

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC TextureAtlas : public Texture
	{
	private:

		Vec2<int> m_cell_size = { 0, 0 };

	public:

		/// <summary>
		/// The same as Texture(const std::string& filepath, const TexSettList& settings = {}) except you specify cell_size.
		/// </summary>
		/// <param name="cell_size">The dimentions (lenght, width) of each cell of the atlas.</param>
		/// @note the dimentions of the texture must be dividable by the cell_size.
		TextureAtlas(const std::string& filepath, const Vec2<int>& cell_size, const TexSettList& settings = {});

		TextureAtlas() = default;

		TextureAtlas(const TextureAtlas&) = delete;
		TextureAtlas& operator=(const TextureAtlas&) = delete;

		TextureAtlas(TextureAtlas&&) noexcept;
		TextureAtlas& operator=(TextureAtlas&&) noexcept;

		const Vec2<int>& get_cell_size() const;
	};

	class BatchQuad;

	/// <summary>
	/// An instance that can gather 250 quads on the same layer at max and render them together.
	/// </summary>
	/// @note A BatchInstance is a Mesh.
	/// If you set the pos, rotation or scale of the BatchInstance,
	/// it will add up with the pos, rotation or scale of each of the BatchQuads.
	class HXG_DECLSPEC BatchInstance : public Mesh
	{
	public:

		BatchInstance() = default;

		BatchInstance(TextureAtlas* texture, Shader* shader = &batching_shader);

		BatchInstance(const BatchInstance&) = delete;
		BatchInstance& operator=(const BatchInstance&) = delete;

		BatchInstance(BatchInstance&&) noexcept;
		BatchInstance& operator=(BatchInstance&&) noexcept;

		/// <summary>
		/// When you're done constructing all the BatchQuads related to this BatchInstance,
		/// call this function in order for all the data to be sent to the gpu.
		/// </summary>
		void construct_batch();

		void draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms = s_empty_uniform_list) override;

		/// <summary>
		/// You can always change the texture atlas,
		/// but it risks to mess up with the texture coordinates of each BatchQuad.
		/// If you change this value, make sure at least that the texture atlas and its cell_size aspect ratio stays unchanged.
		/// </summary>
		TextureAtlas* texture_atlas = nullptr;

	private:

		IndexBuffer m_index_buffer;

		std::vector<uint32_t> m_raw_ib;
		std::vector<float> m_raw_vb;

		std::vector<float> m_uniforms;
		int m_uniforms_id = -1;

		friend class BatchQuad;

		std::vector<BatchQuad*> m_quads;

		void m_add_quad(BatchQuad*, const Vec2<int>& tex_coords);
	};

	class ToBeInit;

	class HXG_DECLSPEC BatchQuad
	{
	public:

		BatchQuad() = default;

		/// @brief Adds a quad to a BatchInstance.
		/// <param name="bi">The BatchInstance you want to push this BatchQuad in.</param>
		/// <param name="tex_coords">The coordinates of a cell in the TextureAtlas of BatchInstance bi</param>
		/// <param name="pos">The position of the quad in the Hexeng2D coordinates system.</param>
		/// <param name="size">Gets multiplied by the size of the texture(cell) to get the size of the quad</param>
		BatchQuad(BatchInstance* bi, const Vec2<int>& tex_coords, const Vec2<int>& pos, float size, float rotation = 0.0f);

		BatchQuad(const BatchQuad&) = delete;
		BatchQuad& operator=(const BatchQuad&) = delete;

		BatchQuad(BatchQuad&&) noexcept;
		BatchQuad& operator=(BatchQuad&&) noexcept;

		/// @note Can always be modified.
		Vec2<int> position = { 0, 0 };

		/// @note Can always be modified.
		float scale = 1.0f;

		/// @note Can always be modified.
		float rotation = 0.0f;

		/// @note Can always be modified.
		Color4 color_filter = Color4::white;

		/// @note Alias to color.A
		float& opacity = color_filter.A;

		/// @note Can always be modified
		Color4 color = {0.0f, 0.0f, 0.0f, 0.0f};

	private:

		friend class BatchInstance;

		BatchInstance* m_batch_instance = nullptr;

		static ToBeInit s_init_vl;
		static VertexLayout s_vertex_layout;
	};

}

#endif