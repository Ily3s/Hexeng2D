#include "Layer.hpp"

namespace Hexeng::Renderer
{

	Layer::Layer() = default;

	std::vector<Layer*> layers;

	Layer::Layer(const std::vector<Mesh*>& mesh_vector, float z_pos, bool is_abs)
		: meshes(mesh_vector), z_position(z_pos), is_absolute(is_abs)
	{
		if (z_pos <= 0)
			throw(std::exception("intended z_pos to be a positive float"));
		layers.push_back(this);
	}

	Layer::Layer(Layer&& other) noexcept
		: meshes(other.meshes), z_position(other.z_position), is_absolute(other.is_absolute)
	{
		auto it = std::find(layers.begin(), layers.end(), &other);
		if (it != layers.end())
			*it = this;
	}

	Layer& Layer::operator=(Layer&& other) noexcept
	{
		meshes = other.meshes;
		z_position = other.z_position;
		is_absolute = other.is_absolute;
		auto it = std::find(layers.begin(), layers.end(), this);
		if (it != layers.end())
			layers.erase(it);
		it = std::find(layers.begin(), layers.end(), &other);
		if (it != layers.end())
			*it = this;
		return *this;
	}

	std::vector<ContextualLayer*> contextual_layers;

	ContextualLayer::ContextualLayer(ContextualLayer&& other) noexcept
		: Layer(std::move(other)), context(other.context)
	{
		auto it = std::find(contextual_layers.begin(), contextual_layers.end(), &other);
		if (it != contextual_layers.end())
			*it = this;
	}

	ContextualLayer& ContextualLayer::operator=(ContextualLayer&& other) noexcept
	{
		Layer::operator=(std::move(other));
		context = other.context;
		auto it = std::find(contextual_layers.begin(), contextual_layers.end(), this);
		if (it != contextual_layers.end())
			contextual_layers.erase(it);
		it = std::find(contextual_layers.begin(), contextual_layers.end(), &other);
		if (it != contextual_layers.end())
			*it = this;
		return *this;
	}

	ContextualLayer::ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition, float z_pos, bool is_abs)
		: Layer(mesh_vector, z_pos, is_abs), context(condition)
	{
		contextual_layers.push_back(this);
	}

	Layer& Layer::operator+=(const Layer& second)
	{
		for (Mesh* r : second.meshes)
			meshes.push_back(r);
		return *this;
	}

	void Layer::load()
	{
		for (auto& rectangle : meshes)
			rectangle->access_texture()->load();
	}

	void Layer::unload()
	{
		for (auto& rectangle : meshes)
		{
			bool is_in_contextual_layer = false;
			for (const ContextualLayer* layer : contextual_layers)
			{
				for (const auto& rec : layer->meshes)
					if (rec->get_texture() == rectangle->get_texture()) is_in_contextual_layer = true;
			}
			if (!is_in_contextual_layer)
				rectangle->access_texture()->unload();
		}
	}

}