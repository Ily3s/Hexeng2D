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
		const Vec2<int>& pos,
		const VertexLayout& layout,
		const IndexBuffer* ib, Texture* tex,
		Shader* shader,
		GLenum type
	)
		:	m_vb(vb, vb_size),
			m_ib(ib),
			m_texture(tex),
			m_shader(shader),
			m_type(type),
			position(pos),
			m_layout(&layout)
	{
		m_vao = { m_vb, layout, *m_ib };
		uniforms.push_back({ &u_transform, &m_transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
		uniforms.push_back({ &u_scale, &scale });
		uniforms.push_back({ &u_color, &color });
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
			m_transform(moving.m_transform),
			rotation(moving.rotation),
			scale(moving.scale),
			m_layout(moving.m_layout),
			color(moving.color),
			opacity(color.A),
			enable(moving.enable)
	{
		enable_ptr = moving.enable_ptr == &moving.enable ? &enable : moving.enable_ptr;

		m_vao = { m_vb, *m_layout, *m_ib };

		for (auto& [ui, value_ptr] : uniforms)
		{
			if (ui == &u_transform)
				value_ptr = &m_transform;
			else if (ui == &u_rotation_angle)
				value_ptr = &rotation;
			else if (ui == &u_scale)
				value_ptr = &scale;
			else if (ui == &u_color)
				value_ptr = &color;
		}
	}

	Mesh& Mesh::operator=(Mesh&& moving) noexcept
	{
		m_vao = std::move(moving.m_vao);
		m_texture = moving.m_texture;
		m_vb = std::move(moving.m_vb);
		m_shader = moving.m_shader;
		m_ib = moving.m_ib;
		uniforms = std::move(moving.uniforms);
		m_type = moving.m_type;
		position = moving.position;
		m_transform = moving.m_transform;
		rotation = moving.rotation;
		scale = moving.scale;
		m_layout = moving.m_layout;
		color = moving.color;
		enable = moving.enable;
		enable_ptr = moving.enable_ptr == &moving.enable ? &enable : moving.enable_ptr;

		m_vao = { m_vb, *m_layout, *m_ib };

		for (auto& [ui, value_ptr] : uniforms)
		{
			if (ui == &u_transform)
				value_ptr = &m_transform;
			else if (ui == &u_rotation_angle)
				value_ptr = &rotation;
			else if (ui == &u_scale)
				value_ptr = &scale;
			else if (ui == &u_color)
				value_ptr = &color;
		}

		return *this;
	}

	std::unordered_map<UniformInterface*, std::vector<void*>> Mesh::s_empty_uniform_list{};

	void Mesh::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		m_shader->bind();

		update_position();

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].push_back(value);

		for (auto& [uniform, values] : parents_uniforms)
			uniform->refresh(m_shader, values);

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].pop_back();

		if (m_texture)
			m_texture->bind();

		m_vao.bind();

		HXG_GL(glDrawElements(m_type, m_ib->get_count(), m_ib->get_type(), nullptr));
	}
	
	void Mesh::update_position()
	{
		m_transform = toCoord(position);
	}

	SuperMesh::SuperMesh(std::vector<Mesh*>&& meshes)
		: meshes(std::move(meshes))
	{
		uniforms.push_back({ &u_transform, &m_transform });
		uniforms.push_back({ &u_rotation_angle, &rotation });
		uniforms.push_back({ &u_scale, &scale });
		uniforms.push_back({ &u_color, &color });
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

	void SuperMesh::draw(std::unordered_map<UniformInterface*, std::vector<void*>>& parents_uniforms)
	{
		if (!*enable_ptr)
			return;

		update_position();

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].push_back(value);

		for (auto& mesh : meshes)
			mesh->draw(parents_uniforms);

		for (auto& [uniform, value] : uniforms)
			parents_uniforms[uniform].pop_back();
	}

	Polygon::Polygon(const std::vector<Vec2<int>>& vertecies, Vec2<int> pos, Color4 color_p, Shader* shader)
	{
		HXG_ASSERT((vertecies.size() > 2),
			HXG_LOG_ERROR("A polygon must be built out of at least three vertecies."););

		color = color_p;

		float* vertex_buffer = new float[vertecies.size() * 2];

		for (size_t i = 0; i < vertecies.size(); i++)
		{
			vertex_buffer[i * 2] = toX(vertecies[i].x);
			vertex_buffer[i * 2 + 1] = toY(vertecies[i].y);
		}

		m_construct_ib(vertecies);

		this->Mesh::operator=(Mesh{ vertex_buffer, vertecies.size() * 2 * sizeof(float), pos, s_vertex_layout, &m_index_buffer, nullptr, shader });

		delete[] vertex_buffer;
	}

	void Polygon::m_construct_ib(const std::vector<Vec2<int>>& vertecies)
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
			std::array<Vec2<int>, 3> triangle_to_test{ vertecies[indexes[pre_index]], vertecies[indexes[i]], vertecies[indexes[post_index]] };
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

	bool Polygon::m_is_acute(const std::array<Vec2<int>, 3>& triangle)
	{
		Vec2<double> a{ triangle[0] - triangle[1] };
		Vec2<double> b{ triangle[2] - triangle[1] };

		double angle = std::atan2(a.x * b.y - a.y * b.x, scalar(a, b));

		return angle >= 0;
	}

	bool Polygon::m_is_in_triangle(Vec2<int> point, const std::array<Vec2<int>, 3>& triangle)
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

}