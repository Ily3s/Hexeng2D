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

	class HXG_DECLSPEC Mesh
	{

	protected:

		friend class SuperMesh;

		GLenum m_type = GL_TRIANGLES;
		VertexArray m_vao;
		VertexBuffer m_vb;
		const IndexBuffer* m_ib = nullptr;
		Texture* m_texture = nullptr;
		Shader* m_shader = nullptr;
		const VertexLayout* m_layout = nullptr;

	public:

		Mesh() = default;

		Mesh(const float* vb, size_t vb_size, const Vec2<int>& pos, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader, GLenum type = GL_TRIANGLES);

		Mesh(Mesh&& moving) noexcept;
		Mesh& operator=(Mesh&& moving) noexcept;

		inline const Texture* get_texture() const { return m_texture; }
		inline const VertexArray* get_vao() const { return &m_vao; }
		inline const Shader* get_shader() const { return m_shader; }
		inline const IndexBuffer* get_ib() const { return m_ib; }
		inline Texture*& access_texture() { return m_texture; }
		inline Shader*& access_shader() { return m_shader; }

		Vec2<int> position{ 0, 0 };
		Vec2<float> transform{ 0.0f, 0.0f };
		float scale = 1.0f;

		float rotation = 0;

		void update_position();

		virtual void draw();

		std::vector<std::pair<UniformInterface*, void*>> uniforms;
	};

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

}

#endif // !MESH_HPP