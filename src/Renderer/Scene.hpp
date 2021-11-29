#ifndef SCENE_HPP
#define SCENE_HPP

#include "Layer.hpp"

namespace Hexeng::Renderer
{

	class Scene
	{

	public:

		std::vector<Layer*> layers;
		float gravity;

		Scene();
		Scene(const std::vector<Layer*>&, float g = 0);

		void load();
		void unload();
	};

	extern std::vector<Scene*> scenes;

}

#endif // !SCENE_HPP