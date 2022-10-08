#ifndef LAYER_HPP
#define LAYER_HPP

#include "../Macros.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Hexeng.hpp"

#include <vector>

namespace Hexeng::Renderer
{

	/// <summary>
	/// Is the layer's position relative to the cam's position or to the center of the screen ?
	/// </summary>
	enum class Position
	{
		/// The position is relative to the center of the screen
		ABSOLUTE,
		/// The position is relative to the cam's position.
		RELATIVE
	};

	/// <summary>
	/// A layer of meshes, at a certain depth z.
	/// </summary>
	class HXG_DECLSPEC Layer
	{
	public:

		/// @note Isn't recommanded to be modified. Still in public to allow some creativity.
		std::vector<Mesh*> meshes;

		/// @note Can always be modified.
		int z_position = 0;

		/**
		* @param z_pos The depth of the Layer
		* @param pos The position mode of the layer (take a look at Position).
		* @param range If set to Range::LOCAL, the layer needs to be added to a scene.
		* If set to Range::GLOBAL, the layer will apear on all scenes.
		*/
		Layer(const std::vector<Mesh*>& mesh_vector, int z_pos, Position pos = Position::RELATIVE, Range range = Range::LOCAL);
		 
		Layer& operator=(const Layer&) = delete;
		Layer(const Layer&) = delete;
		
		Layer& operator=(Layer&&) noexcept;
		Layer(Layer&&) noexcept;
		
		Layer() = default;
		
		virtual void unload();
		virtual void load();

		virtual void draw();

		std::vector<std::pair<UniformInterface*, void*>> uniforms;

	protected:

		Range m_range = Range::LOCAL;

		Position m_position_mode = Position::RELATIVE;
	};

	/// <summary>
	/// A layer that shows up only if a bool is set to true
	/// </summary>
	class HXG_DECLSPEC ContextualLayer : public Layer
	{

	public:

		/// @note Can always be modified.
		bool* context = nullptr;

		ContextualLayer() = default;

		/**
		* @param condition A pointer to the "control variable" of the layer.
		* If the bool it is pointing to is set to false, the layer will not show up, if it is set to true, the layer will show up.
		* @param z_pos The depth of the Layer.
		* @param pos The position mode of the layer (take a look at Position).
		* @param range If set to Range::LOCAL, the layer needs to be added to a scene.
		* If set to Range::GLOBAL, the layer will apear on all scenes.
		*/
		ContextualLayer(const std::vector<Mesh*>& mesh_vector, bool* condition, int z_pos = 1, Position pos = Position::ABSOLUTE, Range range = Range::GLOBAL);

		ContextualLayer& operator=(ContextualLayer&&) noexcept;
		ContextualLayer(ContextualLayer&&) noexcept;

		void draw() override;
	};

	HXG_DECLSPEC std::vector<Mesh*> fusion(std::vector<std::vector<Mesh*>>);

	HXG_DECLSPEC extern std::vector<Layer*> global_layers;

}

#endif // !LAYER_HPP