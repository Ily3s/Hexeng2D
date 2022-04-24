#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../Hexeng.hpp"

namespace Hexeng::Renderer
{

	Mesh::Mesh(const float* vb, const Vec2<int>& pos, const VertexLayout& layout, const IndexBuffer* ib, Texture* tex, Shader* shader, GLenum type)
		:	m_vb(vb, sizeof(vb)),
			m_ib(ib),
			m_texture(tex),
			m_shader(shader),
			m_type(type),
			position(pos)
	{
		m_vao.tie(m_vb, layout, *m_ib);
		uniforms.push_back({ &u_transform, &transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
	}

	Mesh::Mesh(Mesh&& moving) noexcept
		:	m_vao(std::move(moving.m_vao)),
			m_texture(moving.m_texture),
			m_vb(std::move(moving.m_vb)),
			m_shader(moving.m_shader),
			m_ib(std::move(moving.m_ib)),
			uniforms(std::move(moving.uniforms)),
			m_type(moving.m_type),
			position(moving.position),
			transform(moving.transform),
			rotation(moving.rotation) 
	{
		for (auto& [ui, value_ptr] : uniforms)
		{
			if (ui == &Renderer::u_transform)
				value_ptr = &transform;
			if (ui == &Renderer::u_rotation_angle)
				value_ptr = &rotation;
		}
	}

	Mesh& Mesh::operator=(Mesh&& moving) noexcept
	{
		m_vao = std::move(moving.m_vao);
		m_texture = moving.m_texture;
		m_vb = std::move(moving.m_vb);
		m_shader = moving.m_shader;
		m_ib = std::move(moving.m_ib);
		uniforms = std::move(moving.uniforms);
		m_type = moving.m_type;
		position = moving.position;
		transform = moving.transform;
		rotation = moving.rotation;

		for (auto& [ui, value_ptr] : uniforms)
		{
			if (ui == &Renderer::u_transform)
				value_ptr = &transform;
			if (ui == &Renderer::u_rotation_angle)
				value_ptr = &rotation;
		}

		return *this;
	}

	void Mesh::draw()
	{
		m_shader->bind();

		update_position();

		for (auto& [uniform, value] : uniforms)
			uniform->refresh(m_shader, value);

 		if (m_texture)
		{
			m_texture->bind();
		}

		m_vao.bind();

		HXG_GL(glDrawElements(m_type, m_ib->get_count(), m_ib->get_type(), nullptr));

		for (auto& [uniform, value] : uniforms)
			uniform->refresh(m_shader);
	}

	void Mesh::update_position()
	{
		transform = toCoord(position);
	}

	SuperMesh::SuperMesh(std::vector<Mesh*>&& meshes)
		: meshes(std::move(meshes))
	{
		uniforms.push_back({ &u_transform, &transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
	}

	SuperMesh::SuperMesh(SuperMesh&& other) noexcept
		: Mesh(std::move(other))
	{
		meshes = std::move(other.meshes);
	}

	SuperMesh& SuperMesh::operator=(SuperMesh&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		meshes = std::move(other.meshes);

		return *this;
	}

	void SuperMesh::draw()
	{
		for (auto& mesh : meshes)
		{
			for (auto& [ui, val] : uniforms)
			{
				bool not_found = true;

				for (auto& [mesh_ui, mesh_val] : mesh->uniforms)
				{
					if (mesh_ui == ui)
					{
						ui->sum_val(mesh_val, val);
						not_found = false;
						continue;
					}
				}

				if (not_found)
					mesh->uniforms.emplace_back(ui, val);
			}

			mesh->position += position;

			mesh->draw();

			mesh->position -= position;

			for (auto& [ui, val] : uniforms)
			{
				for (auto& [mesh_ui, mesh_val] : mesh->uniforms)
				{
					if (mesh_ui == ui)
					{
						ui->substract_val(mesh_val, val);
						continue;
					}
				}
			}
		}

		for (auto& [ui, val] : uniforms)
			ui->refresh();
	}

}