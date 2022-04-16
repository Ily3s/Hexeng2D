#ifndef LAYER_HPP
#define LAYER_HPP

#include "../Macros.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Hexeng.hpp"

#include <vector>

namespace Hexeng::Renderer
{

	enum class Position
	{
		ABSOLUTE,
		RELATIVE
	};

	class HXG_DECLSPEC Layer
	{

	public:

		std::vector<Mesh*> meshes;
		int z_position = 1;
		Range range = Range::LOCAL;
		Position position_mode = Position::RELATIVE;

		Layer(const std::vector<Mesh*>& mesh_vector, int z_pos, Position pos = Position::RELATIVE, Range range = Range::LOCAL);
		 
		Layer& operator=(const Layer&) = delete;
		Layer(const Layer&) = delete;
		
		Layer& operator=(Layer&&) noexcept;
		Layer(Layer&&) noexcept;
		
		Layer() = default;
		
		virtual void unload();
		virtual void load();

		std::vector<std::pair<UniformInterface*, void*>> uniforms;
	};

	class HXG_DECLSPEC ContextualLayer : public Layer
	{

	public:

		bool* context = nullptr;

		ContextualLayer() = default;
		ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition, int z_pos = 1, Position pos = Position::ABSOLUTE, Range range = Range::GLOBAL);

		ContextualLayer& operator=(ContextualLayer&&) noexcept;
		ContextualLayer(ContextualLayer&&) noexcept;

	};

	HXG_DECLSPEC extern std::vector<Layer*> global_layers;
	HXG_DECLSPEC extern std::vector<ContextualLayer*> global_contextual_layers;

}

#endif // !LAYER_HPP