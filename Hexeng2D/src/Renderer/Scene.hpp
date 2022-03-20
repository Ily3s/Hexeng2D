#ifndef SCENE_HPP
#define SCENE_HPP

#include "Layer.hpp"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC class Scene
	{

	public:

		std::vector<Layer*> layers;
		float gravity;

		HXG_DECLSPEC Scene();
		HXG_DECLSPEC Scene(const std::vector<Layer*>&, float g = 0);

		HXG_DECLSPEC void load();
		HXG_DECLSPEC void unload();
	};

	HXG_DECLSPEC extern std::vector<Scene*> scenes;

}

#endif // !SCENE_HPP