#include <limits>

#include "Layer.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	std::vector<Layer*> global_layers;

	Layer::Layer(const std::vector<Mesh*>& mesh_vector, int z_pos, Position pos, Range range)
		: meshes(mesh_vector),
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
		: meshes(other.meshes),
		z_position(other.z_position),
		uniforms(std::move(other.uniforms)),
		m_range(other.m_range),
		m_position_mode(other.m_position_mode),
		enable(other.enable)
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
		meshes = other.meshes;
		z_position = other.z_position;
		m_position_mode = other.m_position_mode;
		uniforms = std::move(other.uniforms);
		enable = other.enable;

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

	void Layer::load()
	{
		for (auto& mesh : meshes)
		{
			if (mesh->access_texture())
				mesh->access_texture()->load();
		}
	}

	void Layer::unload()
	{
		for (auto& mesh : meshes)
		{
			if (mesh->access_texture())
				mesh->access_texture()->unload();
		}
	}

	void Layer::draw()
	{
		if (!enable)
			return;

		if (z_position < Camera::position.z && m_position_mode == Position::RELATIVE)
			return;

		Camera::s_update_zoom(z_position - Camera::position.z);

		std::unordered_map<UniformInterface*, std::vector<void*>> parents_uniforms;
		for (auto& [uniform, value] : uniforms)
			parents_uniforms.insert({ uniform, {value} });

		for (const auto& mesh : meshes)
			mesh->draw(parents_uniforms);
	}

}