#include "Layer.hpp"

namespace Hexeng::Renderer
{

	Layer::Layer(const std::vector<Mesh*>& mesh_vector)
		: meshes(mesh_vector)
	{
		layers.push_back(this);
	}

	Layer::Layer(Layer&& other) noexcept
		: meshes(other.meshes)
	{
		auto it = std::find(layers.begin(), layers.end(), &other);
		if (it != layers.end())
			*it = this;
	}

	Layer& Layer::operator=(Layer&& other) noexcept
	{
		meshes = other.meshes;
		auto it = std::find(layers.begin(), layers.end(), this);
		if (it != layers.end())
			layers.erase(it);
		it = std::find(layers.begin(), layers.end(), &other);
		if (it != layers.end())
			*it = this;
		return *this;
	}

	ContextualLayer::ContextualLayer(ContextualLayer&& other) noexcept
		: Layer(std::move(other)), context(other.context)
	{
		auto it = std::find(s_contextual_layers.begin(), s_contextual_layers.end(), &other);
		if (it != s_contextual_layers.end())
			*it = this;
	}

	ContextualLayer& ContextualLayer::operator=(ContextualLayer&& other) noexcept
	{
		Layer::operator=(std::move(other));
		context = other.context;
		auto it = std::find(s_contextual_layers.begin(), s_contextual_layers.end(), this);
		if (it != s_contextual_layers.end())
			s_contextual_layers.erase(it);
		it = std::find(s_contextual_layers.begin(), s_contextual_layers.end(), &other);
		if (it != s_contextual_layers.end())
			*it = this;
		return *this;
	}

	std::vector<ContextualLayer*> s_contextual_layers;

	ContextualLayer::ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition)
		: Layer(mesh_vector), context(condition)
	{
		s_contextual_layers.push_back(this);
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