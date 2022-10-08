#ifndef MESH_HPP
#define MESH_HPP

#include "../Macros.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Uniform.hpp"

#include <functional>

namespace Hexeng::Renderer
{

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
		Mesh(const void* vb, size_t vb_size, const Vec2<int>& pos, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader, GLenum type = GL_TRIANGLES);

		Mesh(Mesh&& moving) noexcept;
		Mesh& operator=(Mesh&& moving) noexcept;

		inline const Texture* get_texture() const { return m_texture; }
		inline const VertexArray* get_vao() const { return &m_vao; }
		inline const Shader* get_shader() const { return m_shader; }
		inline const IndexBuffer* get_ib() const { return m_ib; }
		inline Texture*& access_texture() { return m_texture; }
		inline Shader*& access_shader() { return m_shader; }

		/// @note Can be modified at any moment.
		Vec2<int> position{ 0, 0 };

		/// @note Can be modified at any moment.
		float scale = 1.0f;

		/// @note Can be modified at any moment.
		float rotation = 0;

		void update_position();

		virtual void draw();

		std::vector<std::pair<UniformInterface*, void*>> uniforms;

	protected:

		friend class SuperMesh;

		Vec2<float> m_transform{ 0.0f, 0.0f };

		GLenum m_type = GL_TRIANGLES;
		VertexArray m_vao;
		VertexBuffer m_vb;
		const IndexBuffer* m_ib = nullptr;
		Texture* m_texture = nullptr;
		Shader* m_shader = nullptr;
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

		void draw() override;

	};

	// todo : polygon

}

#endif // !MESH_HPP