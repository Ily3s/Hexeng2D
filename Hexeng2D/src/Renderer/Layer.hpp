#ifndef LAYER_HPP
#define LAYER_HPP

#include "../Macros.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <vector>

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Layer
	{

	public:

		std::vector<Mesh*> meshes;
		float z_position = 1.0f;
		bool is_absolute = false;

		Layer(const std::vector<Mesh*>& mesh_vector, float z_pos = 1.0f, bool is_abs = false);
		 
		Layer& operator=(const Layer&) = delete;
		Layer(const Layer&) = delete;
		
		Layer& operator=(Layer&&) noexcept;
		Layer(Layer&&) noexcept;
		
		Layer();
		
		virtual void unload();
		virtual void load();

	};

	class HXG_DECLSPEC ContextualLayer : public Layer
	{

	public:

		bool* context = nullptr;

		ContextualLayer() = default;
		ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition, float z_pos = 1.0f, bool is_abs = true);

		ContextualLayer& operator=(ContextualLayer&&) noexcept;
		ContextualLayer(ContextualLayer&&) noexcept;

	};

	HXG_DECLSPEC extern std::vector<Layer*> layers;
	HXG_DECLSPEC extern std::vector<ContextualLayer*> contextual_layers;

}

#endif // !LAYER_HPP