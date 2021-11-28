#ifndef LAYER_HPP
#define LAYER_HPP

#include "Shader.hpp"
#include "Mesh.hpp"
#include <vector>

namespace Hexeng::Renderer
{

	class Layer
	{

	public:

		std::vector<Mesh*> meshes;

		Layer(const std::vector<Mesh*>& mesh_vector);

		Layer& operator=(const Layer&) = delete;
		Layer(const Layer&) = delete;

		Layer& operator=(Layer&&) noexcept;
		Layer(Layer&&) noexcept;

		Layer() = default;

		virtual void unload();
		virtual void load();

		Layer& operator+=(const Layer& second);

	};

	class ContextualLayer : public Layer
	{

	public:

		bool* context;

		ContextualLayer() = default;
		ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition);

		ContextualLayer& operator=(ContextualLayer&&) noexcept;
		ContextualLayer(ContextualLayer&&) noexcept;

	};

	extern std::vector<Layer*> layers;
	extern std::vector<ContextualLayer*> contextual_layers;

}

#endif // !LAYER_HPP