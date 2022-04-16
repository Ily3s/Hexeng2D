#include <limits>

#include "Layer.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	std::vector<Layer*> global_layers;

	Layer::Layer(const std::vector<Mesh*>& mesh_vector, int z_pos, Position pos, Range range)
		: meshes(mesh_vector), z_position(z_pos), range(range), position_mode(pos)
	{
		if (z_pos <= 0)
			throw(std::exception("intended z_pos to be a positive float"));

		if (position_mode == Position::ABSOLUTE)
		{
			static float one = 1.0f;
			static Vec2<float> zero{ 0.0f, 0.0f };
			uniforms.push_back({ &Camera::u_zoom, &one });
			uniforms.push_back({ &Camera::u_cam, &zero });
			z_position = -INT_MAX;
		}

		if (range == Range::GLOBAL)
			global_layers.push_back(this);
	}

	Layer::Layer(Layer&& other) noexcept
		: meshes(other.meshes), z_position(other.z_position), uniforms(std::move(other.uniforms)), range(other.range), position_mode(other.position_mode)
	{
		if (range == Range::GLOBAL)
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
		position_mode = other.position_mode;
		uniforms = std::move(other.uniforms);

		if (range == Range::GLOBAL)
		{
			auto it = std::find(global_layers.begin(), global_layers.end(), this);
			if (it != global_layers.end())
				global_layers.erase(it);
		}
		if (other.range == Range::GLOBAL)
		{
			auto it = std::find(global_layers.begin(), global_layers.end(), &other);
			if (it != global_layers.end())
				*it = this;
		}
		range = other.range;

		return *this;
	}

	std::vector<ContextualLayer*> global_contextual_layers;

	ContextualLayer::ContextualLayer(ContextualLayer&& other) noexcept
		: Layer(std::move(other)), context(other.context)
	{
		if (range == Range::GLOBAL)
		{
			auto it = std::find(global_contextual_layers.begin(), global_contextual_layers.end(), &other);
			if (it != global_contextual_layers.end())
				*it = this;
		}
	}

	ContextualLayer& ContextualLayer::operator=(ContextualLayer&& other) noexcept
	{
		if (range == Range::GLOBAL)
		{
			auto it = std::find(global_contextual_layers.begin(), global_contextual_layers.end(), this);
			if (it != global_contextual_layers.end())
				global_contextual_layers.erase(it);
		}
		if (other.range == Range::GLOBAL)
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
		: Layer(mesh_vector, z_pos, pos, range), context(condition)
	{
		if (range == Range::GLOBAL)
			global_contextual_layers.push_back(this);
	}

	void Layer::load()
	{
		for (auto& mesh : meshes)
			mesh->access_texture()->load();
	}

	void Layer::unload()
	{
		for (auto& mesh : meshes)
			mesh->access_texture()->unload();
	}

}