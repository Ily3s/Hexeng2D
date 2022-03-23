#ifndef LAYER_HPP
#define LAYER_HPP

#include "../Macros.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <vector>

namespace Hexeng::Renderer
{

	HXG_DECLSPEC class Layer
	{

	public:

		std::vector<Mesh*> meshes;
		float z_position = 1.0f;
		bool is_absolute = false;

		HXG_DECLSPEC Layer(const std::vector<Mesh*>& mesh_vector, float z_pos = 1.0f, bool is_abs = false);
		 
		Layer& operator=(const Layer&) = delete;
		HXG_DECLSPEC Layer(const Layer&) = delete;
		 
		HXG_DECLSPEC Layer& operator=(Layer&&) noexcept;
		HXG_DECLSPEC Layer(Layer&&) noexcept;
		
		HXG_DECLSPEC Layer();
		 
		HXG_DECLSPEC virtual void unload();
		HXG_DECLSPEC virtual void load();

	};

	HXG_DECLSPEC class ContextualLayer : public Layer
	{

	public:

		bool* context;

		ContextualLayer() = default;
		ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition, float z_pos = 1.0f, bool is_abs = true);

		ContextualLayer& operator=(ContextualLayer&&) noexcept;
		ContextualLayer(ContextualLayer&&) noexcept;

	};

	HXG_DECLSPEC extern std::vector<Layer*> layers;
	HXG_DECLSPEC extern std::vector<ContextualLayer*> contextual_layers;

}

#endif // !LAYER_HPP