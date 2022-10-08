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
		meshes = other.meshes;
		z_position = other.z_position;
		m_position_mode = other.m_position_mode;
		uniforms = std::move(other.uniforms);

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

	std::vector<ContextualLayer*> global_contextual_layers;

	ContextualLayer::ContextualLayer(ContextualLayer&& other) noexcept
		: Layer(std::move(other)),
		context(other.context)
	{
		if (m_range == Range::GLOBAL)
		{
			auto it = std::find(global_contextual_layers.begin(), global_contextual_layers.end(), &other);
			if (it != global_contextual_layers.end())
				*it = this;
		}
	}

	ContextualLayer& ContextualLayer::operator=(ContextualLayer&& other) noexcept
	{
		if (m_range == Range::GLOBAL)
		{
			auto it = std::find(global_contextual_layers.begin(), global_contextual_layers.end(), this);
			if (it != global_contextual_layers.end())
				global_contextual_layers.erase(it);
		}
		if (other.m_range == Range::GLOBAL)
		{
			auto it = std::find(global_contextual_layers.begin(), global_contextual_layers.end(), &other);
			if (it != global_contextual_layers.end())
				*it = this;
		}

		Layer::operator=(std::move(other));
		context = other.context;

		return *this;
	}

	ContextualLayer::ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition, int z_pos, Position pos, Range range)
		: Layer(mesh_vector, z_pos, pos, range),
		context(condition)
	{
		if (range == Range::GLOBAL)
			global_contextual_layers.push_back(this);
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
		if (z_position < Camera::position.z && m_position_mode == Position::RELATIVE)
			return;

		Camera::s_update_zoom(z_position - Camera::position.z);

		for (auto& [uniform, value] : uniforms)
			uniform->refresh(value);

		for (const auto& mesh : meshes)
			mesh->draw();

		for (auto& [uniform, value] : uniforms)
			uniform->refresh();
	}

	void ContextualLayer::draw()
	{
		if (!*context)
			return;

		if (z_position < Camera::position.z && m_position_mode == Position::RELATIVE)
			return;

		Camera::s_update_zoom(z_position - Camera::position.z);

		for (auto& [uniform, value] : uniforms)
			uniform->refresh(value);

		for (const auto& mesh : meshes)
			mesh->draw();

		for (auto& [uniform, value] : uniforms)
			uniform->refresh();
	}

}