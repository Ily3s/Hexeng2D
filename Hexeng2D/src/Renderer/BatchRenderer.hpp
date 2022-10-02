#ifndef BATCH_RENDERER_HPP
#define BATCH_RENDERER_HPP

#include "Texture.hpp"
#include "Mesh.hpp"
#include "DefaultShaders.hpp"

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC TextureAtlas : public Texture
	{
	private :

		Vec2<int> m_cell_size = {0, 0};

	public :

		TextureAtlas(const std::string& filepath, const Vec2<int>& cell_size, const TexSettList& settings = {});

		TextureAtlas() = default;

		TextureAtlas(const TextureAtlas&) = delete;
		TextureAtlas& operator=(const TextureAtlas&) = delete;

		TextureAtlas(TextureAtlas&&) noexcept;
		TextureAtlas& operator=(TextureAtlas&&) noexcept;

		const Vec2<int>& get_cell_size() const;
	};

    class BatchQuad;

	class HXG_DECLSPEC BatchInstance : public Mesh
	{
	public :

		BatchInstance() = default;

		BatchInstance(TextureAtlas*, Shader* = &batching_shader);

		BatchInstance(const BatchInstance&) = delete;
		BatchInstance& operator=(const BatchInstance&) = delete;

		BatchInstance(BatchInstance&&) noexcept;
		BatchInstance& operator=(BatchInstance&&) noexcept;

		void construct_batch();

		void draw() override;

		TextureAtlas* texture_atlas = nullptr;

		Color3 color = {0.0f, 0.0f, 0.0f};

	private :

		IndexBuffer m_index_buffer;

		std::vector<uint32_t> m_raw_ib;
		std::vector<float> m_raw_vb;

		std::vector<float> m_uniforms;
		int m_uniforms_id = -1;

		friend class BatchQuad;

		std::vector<BatchQuad*> m_quads;

		void add_quad(BatchQuad*, const Vec2<int>& tex_coords);
	};

	class HXG_DECLSPEC BatchQuad
	{
	public :

		BatchQuad() = default;

		BatchQuad(BatchInstance*, const Vec2<int>& tex_coords, const Vec2<int>& pos, float size, float rotation = 0.0f);

		BatchQuad(const BatchQuad&) = delete;
		BatchQuad& operator=(const BatchQuad&) = delete;

		BatchQuad(BatchQuad&&) noexcept;
		BatchQuad& operator=(BatchQuad&&) noexcept;

		Vec2<int> position = {0, 0};
		float scale = 1.0f;
		float rotation = 0.0f;

		static VertexLayout vertex_layout;

	private :

		friend class BatchInstance;

		BatchInstance* m_batch_instance = nullptr;
	};

}

#endif