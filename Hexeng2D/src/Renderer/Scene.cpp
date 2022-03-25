#include "Scene.hpp"
#include <algorithm>

namespace Hexeng::Renderer
{

	std::unordered_map<int, Scene*> scenes;
	int scene_id = 0;

	Scene::Scene(int id, const std::vector<Layer*>& l)
		: layers(l)
	{
		std::sort(layers.begin(), layers.end(), [](Layer* layer1, Layer* layer2) {
			return layer1->z_position > layer2->z_position; });
		scenes.insert_or_assign(id, this);
	}

	void Scene::load()
	{
		for (auto layer : layers)
			layer->load();
	}

	void Scene::unload()
	{
		for (auto layer : layers)
			layer->unload();
	}

}