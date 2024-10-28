#ifndef BATCH_RENDERER_HPP
#define BATCH_RENDERER_HPP

#include "Texture.hpp"
#include "Mesh.hpp"
#include "DefaultShaders.hpp"
#include "Color.hpp"

#include <unordered_map>
#include <vector>

/// @file There should be at least one TextureAtlas per TALayout and at most one TALayout per BatchInstance

namespace Hexeng::Renderer
{

	/// @brief A Shader made for BatchInstance, supporting n quads.
	class HXG_DECLSPEC BatchingShader : public Shader
	{
	public :

		/// @param size The maximum number of quads this shader will support on BatchInstance
		/// @exception std::runtime_error if size > BatchInstance::get_max_quads()
		BatchingShader(size_t size);

		BatchingShader() = default;

		BatchingShader(const BatchingShader&) = delete;
		BatchingShader& operator=(const BatchingShader&) = delete;

		BatchingShader(BatchingShader&&) noexcept;
		BatchingShader& operator=(BatchingShader&&) noexcept;
	};

    /// @note stands for TextureAtlasLayout
    struct HXG_DECLSPEC TALayout
    {
    public :

        /// @brief builds a custom layout from an RGBA png where the colors identify the cells.
        /// @note Cells must be rectangles.
        /// @note There should be no linear filtering on the provided png (ie sharp cell borders)
        TALayout(const std::string& filepath);

        /// @brief builds a grid-like layout where all cells are the same size.
        /// @param cell_size The size of each cell
        /// @param grid_size The size of the grid in terms of cells
        TALayout(Vec2<int> cell_size, Vec2<int> grid_size);
                    
	    TALayout() = default;

	    TALayout(const TALayout&) = default;
	    TALayout& operator=(const TALayout&) = default;

    private :

        friend class BatchInstance;
        friend class BatchQuad;
        fTexCoord get_coords(Color4 id);
        fTexCoord get_coords(Vec2<int> id);

        bool m_is_grid = false;

        Vec2<int> m_size = { 0, 0 };
        Vec2<int> m_cell_size = { 0, 0 };

        std::unordered_map<uint32_t, fTexCoord> m_map;
    };

	class HXG_DECLSPEC TextureAtlas : public Texture
	{
	public:

		/// @brief Creates a texture for a BatchInstance; same constructor as for Texture
		/// @note MIN_FILTER and MAG_FILTER have to be the same.
		TextureAtlas(const std::string& filepath, const TexSettList& settings = {});

		TextureAtlas() = default;

		TextureAtlas(const TextureAtlas&) = delete;
		TextureAtlas& operator=(const TextureAtlas&) = delete;

		TextureAtlas(TextureAtlas&&) noexcept;
		TextureAtlas& operator=(TextureAtlas&&) noexcept;

        static Uniform<float> u_bleeding_correction;
    private:

        friend class BatchInstance;
        float m_bleeding_correction = 0.5;
	};

	class BatchQuad;

	/// @brief An instance that can gather n quads on the same layer and render them together.
	/// @note A BatchInstance is a Mesh.
	/// If you set the pos, rotation or scale of the BatchInstance,
	/// it will add up with the pos, rotation or scale of each of the BatchQuads.
    /// @note BatchInstance::texture must be a TextureAtlas*
	class HXG_DECLSPEC BatchInstance : public Mesh
	{
	public:

		BatchInstance() = default;

		BatchInstance(TALayout layout, Shader* shader = &batching_shader_250);

		BatchInstance(const BatchInstance&) = delete;
		BatchInstance& operator=(const BatchInstance&) = delete;

		BatchInstance(BatchInstance&&) noexcept;
		BatchInstance& operator=(BatchInstance&&) noexcept;

		/// When you're done constructing all the BatchQuads related to this BatchInstance,
		/// call this function in order for all the data to be sent to the gpu.
		void construct_batch();

		void draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms = s_empty_uniform_list) override;

		/// @brief Returns the maximum BatchQuad ammout a BatchInstance can handle on the hardware this software is running on.
		static int get_max_quads();

	private:

        TALayout m_layout;

		IndexBuffer m_index_buffer;

		std::vector<uint32_t> m_raw_ib;
		std::vector<float> m_raw_vb;

		std::vector<float> m_uniforms;
		int m_uniforms_id = -1;

		friend class BatchQuad;

		std::vector<BatchQuad*> m_quads;

		void m_add_quad(BatchQuad*, fTexCoord);
	};

	class ToBeInit;

	class HXG_DECLSPEC BatchQuad
	{
	public:

		BatchQuad() = default;

		/// @brief Adds a quad to a BatchInstance.
		/// <param name="bi">The BatchInstace you want to push this BatchQuad in.</param>
		/// <param name="cell_coords">The coordinates of a cell in the TextureAtlas of BatchInstance bi</param>
		/// <param name="pos">The position of the quad in the Hexeng2D coordinates system.</param>
		/// <param name="size">A scale factor applied to the size of the cell in the layout</param>
        /// @note Can only be used with a grid-like TALayout
		BatchQuad(BatchInstance* bi, Vec2<int> cell_coords, Vec2<float> pos, float size, float rotation = 0.0f);

		/// @brief Adds a quad to a BatchInstance.
		/// <param name="bi">The BatchInstace you want to push this BatchQuad in.</param>
		/// <param name="id">The color identifying the texture cell in TALayout</param>
		/// <param name="pos">The position of the quad in the Hexeng2D coordinates system.</param>
		/// <param name="size">A scale factor applied to the size of the cell in the layout</param>
        /// @note Cannot be used with a grid-like TALayout
		BatchQuad(BatchInstance* bi, Color4 id, Vec2<float> pos, float size, float rotation = 0.0f);

        /// @brief A shortcut for BatchQuad(BatchInstance*, Color4, Vec2<float>, float, float)
		BatchQuad(BatchInstance* bi, const std::string& color_id, Vec2<float> pos, float size, float rotation = 0.0f);

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
