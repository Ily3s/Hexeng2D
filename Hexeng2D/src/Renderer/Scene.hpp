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

		HXG_DECLSPEC Scene(int id, const std::vector<Layer*>& layers);

		HXG_DECLSPEC void load();
		HXG_DECLSPEC void unload();
	};

	HXG_DECLSPEC extern std::unordered_map<int, Scene*> scenes;
	HXG_DECLSPEC extern int scene_id;

}

#endif // !SCENE_HPP