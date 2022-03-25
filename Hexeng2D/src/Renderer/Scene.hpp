#ifndef SCENE_HPP
#define SCENE_HPP

#include "Layer.hpp"
#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	class HXG_DECLSPEC Scene
	{

	public:

		std::vector<Layer*> layers;

		Scene(int id, const std::vector<Layer*>& layers);

		void load();
		void unload();
	};

	HXG_DECLSPEC extern std::unordered_map<int, Scene*> scenes;
	HXG_DECLSPEC extern int scene_id;

}

#endif // !SCENE_HPP