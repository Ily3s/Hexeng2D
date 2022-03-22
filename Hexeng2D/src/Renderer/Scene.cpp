#include "Scene.hpp"

namespace Hexeng::Renderer
{

	std::unordered_map<int, Scene*> scenes;
	int scene_id = 0;

	Scene::Scene(int id, const std::vector<Layer*>& l)
		: layers(l)
	{
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