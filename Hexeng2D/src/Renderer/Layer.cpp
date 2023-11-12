#include <limits>

#include "Layer.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	std::vector<Layer*> global_layers;

	Layer::Layer(std::vector<Mesh*>&& mesh_vector, int z_pos, Position pos, Range range)
		: SuperMesh(std::move(mesh_vector)),
		z_position(z_pos),
		m_range(range),
		m_position_mode(pos)
	{
		if (m_position_mode == Position::ABSOLUTE)
		{
			static float one = 1.0f;
			static Vec2<float> zero{ 0.0f, 0.0f };
			uniforms.push_back({ &Camera::u_zoom, &one });
			uniforms.push_back({ &Camera::u_cam, &zero });
		}

		if (range == Range::GLOBAL)
			global_layers.push_back(this);
	}

	Layer::Layer(Layer&& other) noexcept
		: SuperMesh(std::move(other)),
		z_position(other.z_position),
		m_range(other.m_range),
		m_position_mode(other.m_position_mode)
	{
		if (m_range == Range::GLOBAL)
		{
			auto it = std::find(global_layers.begin(), global_layers.end(), &other);
			if (it != global_layers.end())
				*it = this;
		}
	}

	Layer& Layer::operator=(Layer&& other) noexcept
	{
		SuperMesh::operator=(std::move(other));
		z_position = other.z_position;
		m_position_mode = other.m_position_mode;

		if (m_range == Range::GLOBAL)
		{
			auto it = std::find(global_layers.begin(), global_layers.end(), this);
			if (it != global_layers.end())
				global_layers.erase(it);
		}
		if (other.m_range == Range::GLOBAL)
		{
			auto it = std::find(global_layers.begin(), global_layers.end(), &other);
			if (it != global_layers.end())
				*it = this;
		}
		m_range = other.m_range;

		return *this;
	}

	HXG_DECLSPEC std::vector<Mesh*> fusion(std::vector<std::vector<Mesh*>> bidim_v)
	{
		std::vector<Mesh*> output;
		for (auto& v : bidim_v)
		{
			for (auto& mesh : v)
				output.push_back(mesh);
		}
		return output;
	}

	void Layer::draw()
	{
		if (!*enable_ptr)
			return;

		if (z_position < Camera::position.z && m_position_mode == Position::RELATIVE)
			return;

		update_position();

		if (m_position_mode == Position::RELATIVE)
			Camera::s_update_zoom(z_position - Camera::position.z);

		std::unordered_map<UniformInterface*, std::vector<void*>> parents_uniforms;
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
	}

}