#ifndef MESH_HPP
#define MESH_HPP

#include <functional>
#include <unordered_map>
#include <vector>

#include "../Macros.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Uniform.hpp"
#include "../Color.hpp"
#include "DefaultShaders.hpp"

namespace Hexeng::Renderer
{

	struct BlendingMethod
	{
		GLenum src = GL_SRC_ALPHA;
		GLenum dest = GL_ONE_MINUS_SRC_ALPHA;
	};

	/// @note It isn't recommanded that you use this class directly. Instead you may want to use a derived class.
	class HXG_DECLSPEC Mesh
	{
	public:

		Mesh() = default;

		/// <param name="vb">The vertex buffer data.</param>
		/// <param name="vb_size">the size (in bytes) of the vertex buffer.</param>
		/// <param name="pos">the position of the Mesh (in the Hexeng2D coordinate system)</param>
		/// <param name="layout">See VertexLayout</param>
		/// <param name="ib">Take a look at IndexBuffer</param>
		/// <param name="tex">Take a look at Texture</param>
		/// <param name="shader">Take a look at Shader</param>
		/// <param name="type">The type of geometry you want to draw. By default, it's set to GL_TRIANGLES</param>
		Mesh(const void* vb, size_t vb_size, const Vec2<float>& pos, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader, GLenum type = GL_TRIANGLES);

		Mesh(Mesh&& moving) noexcept;
		Mesh& operator=(Mesh&& moving) noexcept;

		inline const VertexArray* get_vao() const { return &m_vao; }
		inline const IndexBuffer* get_ib() const { return m_ib; }

		/// @note Can be modified at any moment.
		Vec2<float> position{ 0, 0 };

		/// @note Can be modified at any moment.
		float scale = 1.0f;

		/// @note Can be modified at any moment.
		float rotation = 0;

		/// @brief Absorbtion color.
		/// @note Can be modified at any moment.
		Color4 color_filter = Color4::white;

		/// @note Alias to color.A
		float& opacity = color_filter.A;

		/// @brief Emission color.
		/// @note Can be modified at any moment
		Color4 color = {0.0f, 0.0f, 0.0f, 0.0f};

        /// @note Can be modified at any moment.
        Texture* texture = nullptr;
        
		/// @note Can be modified at any moment. Useless if enable_ptr is overwritten
		bool enable = true;

		/// @note Can be modified at any moment.
		bool* enable_ptr = &enable;

        /// @note Can be modified at any moment.
        Shader* shader = nullptr;

		/// @note Can be modified at any moment.
		BlendingMethod blending_method = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };

		void update_position();

		virtual void draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms = s_empty_uniform_list);

		std::vector<std::pair<UniformInterface*, void*>> uniforms = {};

		virtual void load();
		virtual void unload();

		inline const GLenum& get_type() const { return m_type; }

	protected: // Almost everything in that field can only be modified at Renderer init

		static std::unordered_map<UniformInterface*, std::vector<void*>> s_empty_uniform_list;

		friend class SuperMesh;
		friend class MeshCopy;

		Vec2<float> m_transform{ 0.0f, 0.0f };

		GLenum m_type = GL_TRIANGLES;
		VertexArray m_vao;
		VertexBuffer m_vb;
		const IndexBuffer* m_ib = nullptr;
		const VertexLayout* m_layout = nullptr;
	};

	/// <summary>
	/// A bunch of meshes that shares some attributes.
	/// For example, a player got legs and arms that can move individually,
	/// but the position of the legs and arms are relative to the position of the body.
	/// In this example, legs and arms are Meshes, the body is a SuperMesh.
	/// </summary>
	/// @note A SuperMesh can be made up of other SuperMeshes.
	/// Also a SuperMesh is NOT batch rendered. If you're looking for batch rendering, take a look at BatchInstance instead.
	class HXG_DECLSPEC SuperMesh : public Mesh
	{
	public :

		std::vector<Mesh*> meshes;

		SuperMesh(std::vector<Mesh*>&& meshes);

		SuperMesh() = default;

		SuperMesh(const SuperMesh&) = delete;
		SuperMesh& operator=(const SuperMesh&) = delete;

		SuperMesh(SuperMesh&&) noexcept;
		SuperMesh& operator=(SuperMesh&&) noexcept;

		void draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms = s_empty_uniform_list) override;

		virtual void load() override;
		virtual void unload() override;

		/// @note scale all the submeshes individually
		float inner_scale = 1.0f;

		/// @note rotate all the submeshes individually
		float inner_rotation = 0.0f;

	protected :

		void m_update_childs_positions(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms);
	};

	class ToBeInit;

	class HXG_DECLSPEC Polygon : public Mesh
	{
	public :

		/// <param name="vertecies">All vertecies coordinates in the Hexeng2D coordinate system, in clockwise order.</param>
		/// @note If you want to make a custom shader for the polygon,
		/// take a look at DefaultShaders.glsl / .hpp / .cpp, copy paste poly_shader and modify it as you want.
		Polygon(const std::vector<Vec2<float>>& vertecies, Vec2<float> pos = { 0, 0 }, Color4 color = Color4::white, Shader* = &poly_shader);

		Polygon() = default;

		Polygon(const Polygon&) = delete;
		Polygon& operator=(const Polygon&) = delete;

		Polygon(Polygon&&) noexcept;
		Polygon& operator=(Polygon&&) noexcept;

		inline static const VertexLayout& get_vertex_layout() { return s_vertex_layout; }

	private :

		IndexBuffer m_index_buffer;

		void m_construct_ib(const std::vector<Vec2<float>>& vertecies);

		static bool m_is_acute(const std::array<Vec2<float>, 3>& triangle);
		static bool m_is_in_triangle(Vec2<float> point, const std::array<Vec2<float>, 3>& triangle);

		static VertexLayout s_vertex_layout;
		static ToBeInit s_init_layout;
	};

	/// @brief A "copy", "link" to a Mesh.
	/// @details It can have different uniforms (position, color, texture ...) than the original mesh but the vertex array object stays the same.
	/// If the original mesh gets destructed, then all copies of it become unvalid (as they are more like links than like copies).
	class HXG_DECLSPEC MeshCopy : public Mesh
	{
	public :

		MeshCopy(const Mesh&);
		MeshCopy& operator=(const Mesh&);

		MeshCopy(const MeshCopy&);
		MeshCopy& operator=(const MeshCopy&);

		MeshCopy() = default;

		MeshCopy(MeshCopy&&) noexcept;
		MeshCopy& operator=(MeshCopy&&) noexcept;

		void draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms = s_empty_uniform_list) override;

	private :

		const VertexArray* m_vao_ptr = nullptr;
	};

}

#endif // !MESH_HPP
