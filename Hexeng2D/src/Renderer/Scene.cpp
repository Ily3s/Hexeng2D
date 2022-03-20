#include "Scene.hpp"

namespace Hexeng::Renderer
{

	std::vector<Scene*> scenes;

	Scene::Scene()
	{
		scenes.push_back(this);
	}

	Scene::Scene(const std::vector<Layer*>& l, float g)
		: layers(l), gravity(g)
	{
		scenes.push_back(this);
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