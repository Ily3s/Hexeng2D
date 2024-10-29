#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../Hexeng.hpp"

#include <array>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Hexeng::Renderer
{

	Mesh::Mesh
	(
		const void* vb,
		size_t vb_size,
		const Vec2<float>& pos,
		const VertexLayout& layout,
		const IndexBuffer* ib, Texture* tex,
		Shader* shader,
		GLenum type
	)
		:	m_vb(vb, vb_size),
			m_ib(ib),
			texture(tex),
			shader(shader),
			m_type(type),
			position(pos),
			m_layout(&layout)
	{
		m_vao = { m_vb, layout, *m_ib };
		uniforms.push_back({ &u_transform, &m_transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
		uniforms.push_back({ &u_scale, &scale });
		uniforms.push_back({ &u_color_filter, &color_filter });
		uniforms.push_back({ &u_color, &color });
	}

	Mesh::Mesh(Mesh&& moving) noexcept
		:	m_vao(std::move(moving.m_vao)),
			texture(moving.texture),
			m_vb(std::move(moving.m_vb)),
			shader(moving.shader),
			m_ib(std::move(moving.m_ib)),
			uniforms(std::move(moving.uniforms)),
			m_type(moving.m_type),
			position(moving.position),
			m_transform(moving.m_transform),
			rotation(moving.rotation),
			scale(moving.scale),
			m_layout(moving.m_layout),
			color_filter(moving.color_filter),
			opacity(color_filter.A),
			enable(moving.enable),
			color(moving.color),
			blending_method(moving.blending_method)
	{
		enable_ptr = moving.enable_ptr == &moving.enable ? &enable : moving.enable_ptr;

		if(moving.m_vao.is_init())
			m_vao = { m_vb, *m_layout, *m_ib };

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &moving.m_transform)
				value_ptr = &m_transform;
			else if (value_ptr == &moving.rotation)
				value_ptr = &rotation;
			else if (value_ptr == &moving.scale)
				value_ptr = &scale;
			else if (value_ptr == &moving.color_filter)
				value_ptr = &color_filter;
			else if (value_ptr == &moving.color)
				value_ptr = &color;
		}
	}

	Mesh& Mesh::operator=(Mesh&& moving) noexcept
	{
		m_vao = std::move(moving.m_vao);
		texture = moving.texture;
		m_vb = std::move(moving.m_vb);
		shader = moving.shader;
		m_ib = moving.m_ib;
		uniforms = std::move(moving.uniforms);
		m_type = moving.m_type;
		position = moving.position;
		m_transform = moving.m_transform;
		rotation = moving.rotation;
		scale = moving.scale;
		m_layout = moving.m_layout;
		color_filter = moving.color_filter;
		enable = moving.enable;
		color = moving.color;
		blending_method = moving.blending_method;
		enable_ptr = moving.enable_ptr == &moving.enable ? &enable : moving.enable_ptr;

		if(moving.m_vao.is_init())
			m_vao = { m_vb, *m_layout, *m_ib };

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &moving.m_transform)
				value_ptr = &m_transform;
			else if (value_ptr == &moving.rotation)
				value_ptr = &rotation;
			else if (value_ptr == &moving.scale)
				value_ptr = &scale;
			else if (value_ptr == &moving.color_filter)
				value_ptr = &color_filter;
			else if (value_ptr == &moving.color)
				value_ptr = &color;
		}

		return *this;
	}

	std::unordered_map<UniformInterface*, std::vector<void*>> Mesh::s_empty_uniform_list{};

	void Mesh::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		shader->bind();

		if (!texture)
			Texture::unbind();

		HXG_GL(glBlendFunc(blending_method.src, blending_method.dest));

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].push_back(value);

		for (auto& [uniform, values] : parents_uniforms)
			uniform->refresh(shader, values);

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].pop_back();

		if (texture)
			texture->bind();

		m_vao.bind();

		HXG_GL(glDrawElements(m_type, m_ib->get_count(), m_ib->get_type(), nullptr));
	}
	
	void Mesh::update_position()
	{
		m_transform = toCoord(position);
	}

	void Mesh::load()
	{
		if (texture)
			texture->load();
	}

	void Mesh::unload()
	{
		if (texture)
			texture->load();
	}

	SuperMesh::SuperMesh(std::vector<Mesh*>&& meshes)
		: meshes(std::move(meshes))
	{
		uniforms.push_back({ &u_transform, &m_transform });
		uniforms.push_back({ &u_color_filter, &color_filter });
		uniforms.push_back({ &u_color, &color });
		uniforms.push_back({ &u_scale, &inner_scale });
		uniforms.push_back({ &u_rotation_angle, &inner_rotation });
	}

	SuperMesh::SuperMesh(SuperMesh&& other) noexcept
		: Mesh(std::move(other))
	{
		meshes = std::move(other.meshes);
		inner_scale = other.inner_scale;
		inner_rotation = other.inner_rotation;

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &other.inner_rotation)
				value_ptr = &inner_rotation;
			else if (value_ptr == &other.inner_scale)
				value_ptr = &inner_scale;
		}
	}

	SuperMesh& SuperMesh::operator=(SuperMesh&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		meshes = std::move(other.meshes);
		inner_scale = other.inner_scale;
		inner_rotation = other.inner_rotation;

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &other.inner_rotation)
				value_ptr = &inner_rotation;
			else if (value_ptr == &other.inner_scale)
				value_ptr = &inner_scale;
		}

		return *this;
	}

	void SuperMesh::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].push_back(value);

		m_update_childs_positions(parents_uniforms);

		for (auto& mesh : meshes)
		{
			float mesh_scale = mesh->scale;
			float mesh_rota = mesh->rotation;
			mesh->scale *= scale;
			mesh->rotation += rotation;
			mesh->draw(parents_uniforms);
			mesh->rotation = mesh_rota;
			mesh->scale = mesh_scale;
		}

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].pop_back();
	}

	void SuperMesh::m_update_childs_positions(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		double angle = rotation * (M_PI / 180.0);
		for (auto mesh : meshes)
		{
			mesh->m_transform = {toX(mesh->position.x * cos(angle) + mesh->position.y * sin(angle)),
								toY(mesh->position.y * cos(angle) - mesh->position.x * sin(angle))};

			mesh->m_transform *= scale;
		}
	}

	void SuperMesh::unload()
	{
		for (auto mesh : meshes)
			mesh->unload();
	}

	void SuperMesh::load()
	{
		for (auto mesh : meshes)
			mesh->load();
	}

	Polygon::Polygon(const std::vector<Vec2<float>>& vertecies, Vec2<float> pos, Color4 color_p, Shader* shader)
	{
		HXG_ASSERT((vertecies.size() > 2),
			HXG_LOG_ERROR("A polygon must be built out of at least three vertecies."););

		color_filter = color_p;

		float* vertex_buffer = new float[vertecies.size() * 2];

		for (size_t i = 0; i < vertecies.size(); i++)
		{
			vertex_buffer[i * 2] = vertecies[i].x;
			vertex_buffer[i * 2 + 1] = vertecies[i].y;
		}

		m_construct_ib(vertecies);

		this->Mesh::operator=(Mesh{ vertex_buffer, vertecies.size() * 2 * sizeof(float), pos, s_vertex_layout, &m_index_buffer, nullptr, shader });

		delete[] vertex_buffer;
	}

	void Polygon::m_construct_ib(const std::vector<Vec2<float>>& vertecies)
	{
		std::vector<int> indexes;
		std::vector<unsigned int> triangles_indexes;

		indexes.reserve(vertecies.size());
		for (int i = 0; i < vertecies.size(); i++)
			indexes.push_back(i);

		triangles_indexes.reserve((vertecies.size() - 2) * 3);

		for (int i = 0; i < vertecies.size(); i++)
		{
			if (indexes.size() == 3)
			{
				triangles_indexes.push_back(indexes[0]);
				triangles_indexes.push_back(indexes[1]);
				triangles_indexes.push_back(indexes[2]);

				break;
			}

			int pre_index = i ? i - 1 : indexes.size() - 1;
			int post_index = i < indexes.size() - 1 ? i + 1 : 0;
			std::array<Vec2<float>, 3> triangle_to_test{ vertecies[indexes[pre_index]], vertecies[indexes[i]], vertecies[indexes[post_index]] };
			if (m_is_acute(triangle_to_test))
			{
				bool is_everything_fine = true;
				for (int j = 0; j < vertecies.size(); j++)
				{
					if (j == indexes[i] || j == indexes[pre_index] || j == indexes[post_index])
						break;

					if (m_is_in_triangle(vertecies[j], triangle_to_test))
					{
						is_everything_fine = false;
						break;
					}
				}
				if (is_everything_fine)
				{
					triangles_indexes.push_back(indexes[pre_index]);
					triangles_indexes.push_back(indexes[i]);
					triangles_indexes.push_back(indexes[post_index]);

					indexes.erase(indexes.begin() + i);

					i = -1;
				}
			}
		}

		m_index_buffer = IndexBuffer(&triangles_indexes[0], GL_UNSIGNED_INT, triangles_indexes.size());
	}

	bool Polygon::m_is_acute(const std::array<Vec2<float>, 3>& triangle)
	{
		Vec2<double> a{ triangle[0] - triangle[1] };
		Vec2<double> b{ triangle[2] - triangle[1] };

		double angle = std::atan2(a.x * b.y - a.y * b.x, scalar(a, b));

		return angle >= 0;
	}

	bool Polygon::m_is_in_triangle(Vec2<float> point, const std::array<Vec2<float>, 3>& triangle)
	{
		for (int i = 0; i < 3; i++)
		{
			int post_index = i < 2 ? i + 1 : 0;

			Vec2<double> edge{ triangle[post_index] - triangle[i] };
			Vec2<double> to_point{ point - triangle[i] };

			double angle_to_point = std::atan2(to_point.x * edge.y - to_point.y * edge.x, scalar(to_point, edge));

			if (angle_to_point < 0)
				return false;
		}

		return true;
	}

	Polygon::Polygon(Polygon&& other) noexcept
		: Mesh(std::move(other))
	{
		m_index_buffer = std::move(other.m_index_buffer);
		m_ib = &m_index_buffer;
	}

	Polygon& Polygon::operator=(Polygon&& other) noexcept
	{
        Mesh::operator=(std::move(other));

		m_index_buffer = std::move(other.m_index_buffer);
		m_ib = &m_index_buffer;

		return *this;
	}

	VertexLayout Polygon::s_vertex_layout;

	ToBeInit Polygon::s_init_layout{ std::function<void(void)> { []() {
		Polygon::s_vertex_layout = VertexLayout({ {2, GL_FLOAT} });
	} } };

	MeshCopy::MeshCopy(const Mesh& other)
		: m_vao_ptr(other.get_vao())
	{
		texture = other.texture;
		shader = other.shader;
		blending_method = other.blending_method;
		m_ib = other.get_ib();
		m_type = other.get_type();
		uniforms = other.uniforms;

		color = other.color;
		color_filter = other.color_filter;
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;

		enable = other.enable;
		enable_ptr = &other.enable == other.enable_ptr ? &enable : other.enable_ptr;

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &other.m_transform)
				value_ptr = &m_transform;
			else if (value_ptr == &other.rotation)
				value_ptr = &rotation;
			else if (value_ptr == &other.scale)
				value_ptr = &scale;
			else if (value_ptr == &other.color_filter)
				value_ptr = &color_filter;
			else if (value_ptr == &other.color)
				value_ptr = &color;
		}
	}

	MeshCopy& MeshCopy::operator=(const Mesh& other)
	{
		m_vao_ptr = other.get_vao();
		uniforms = other.uniforms;

		texture = other.texture;
		shader = other.shader;
		blending_method = other.blending_method;
		m_ib = other.get_ib();
		m_type = other.get_type();

		color = other.color;
		color_filter = other.color_filter;
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;

		enable = other.enable;
		enable_ptr = &other.enable == other.enable_ptr ? &enable : other.enable_ptr;

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &other.m_transform)
				value_ptr = &m_transform;
			else if (value_ptr == &other.rotation)
				value_ptr = &rotation;
			else if (value_ptr == &other.scale)
				value_ptr = &scale;
			else if (value_ptr == &other.color_filter)
				value_ptr = &color_filter;
			else if (value_ptr == &other.color)
				value_ptr = &color;
		}

		return *this;
	}

	MeshCopy::MeshCopy(const MeshCopy& other)
		: m_vao_ptr(other.m_vao_ptr)
	{
		texture = other.texture;
		shader = other.shader;
		blending_method = other.blending_method;
		m_ib = other.m_ib;
		m_type = other.m_type;
		uniforms = other.uniforms;

		color = other.color;
		color_filter = other.color_filter;
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;

		enable = other.enable;
		enable_ptr = &other.enable == other.enable_ptr ? &enable : other.enable_ptr;

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &other.m_transform)
				value_ptr = &m_transform;
			else if (value_ptr == &other.rotation)
				value_ptr = &rotation;
			else if (value_ptr == &other.scale)
				value_ptr = &scale;
			else if (value_ptr == &other.color_filter)
				value_ptr = &color_filter;
			else if (value_ptr == &other.color)
				value_ptr = &color;
		}
	}

	MeshCopy& MeshCopy::operator= (const MeshCopy& other)
	{
		m_vao_ptr = other.m_vao_ptr;
		uniforms = other.uniforms;

		texture = other.texture;
		shader = other.shader;
		blending_method = other.blending_method;
		m_ib = other.m_ib;
		m_type = other.m_type;

		color = other.color;
		color_filter = other.color_filter;
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;

		enable = other.enable;
		enable_ptr = &other.enable == other.enable_ptr ? &enable : other.enable_ptr;

		for (auto& [_, value_ptr] : uniforms)
		{
			if (value_ptr == &other.m_transform)
				value_ptr = &m_transform;
			else if (value_ptr == &other.rotation)
				value_ptr = &rotation;
			else if (value_ptr == &other.scale)
				value_ptr = &scale;
			else if (value_ptr == &other.color_filter)
				value_ptr = &color_filter;
			else if (value_ptr == &other.color)
				value_ptr = &color;
		}

		return *this;
	}

	MeshCopy::MeshCopy(MeshCopy&& other) noexcept
		: Mesh(std::move(other)), m_vao_ptr(other.m_vao_ptr) 
	{
		other.m_vao_ptr = nullptr;
	}

	MeshCopy& MeshCopy::operator=(MeshCopy&& other) noexcept
	{
		Mesh::operator=(std::move(other));
		m_vao_ptr = other.m_vao_ptr;
		other.m_vao_ptr = nullptr;
		return *this;
	}

	void MeshCopy::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		shader->bind();

		if (!texture)
			Texture::unbind();

		HXG_GL(glBlendFunc(blending_method.src, blending_method.dest));

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].push_back(value);

		for (auto& [uniform, values] : parents_uniforms)
			uniform->refresh(shader, values);

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].pop_back();

		if (texture)
			texture->bind();

		m_vao_ptr->bind();

		HXG_GL(glDrawElements(m_type, m_ib->get_count(), m_ib->get_type(), nullptr));
	}

}
