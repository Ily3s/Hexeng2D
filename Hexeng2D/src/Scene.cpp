#include <algorithm>

#include "Scene.hpp"
#include "Renderer/Renderer.hpp"

namespace Hexeng
{

	std::unordered_map<int, Scene*> scenes;
	int scene_id = 0;

	Scene::Scene(int id, const std::unordered_map<SceneComponent, std::vector<void*>>& components)
	{
		for (auto& [type, component] : components)
		{
			switch (type)
			{
			case SceneComponent::LAYERS :
				for (Renderer::Layer* layer : *(std::vector<Renderer::Layer*>*)& component)
					layers.push_back(layer);
				break;

			case SceneComponent::EVENTS :
				for (EventManager::Event* evt : *(std::vector<EventManager::Event*>*)&component)
					events.push_back(evt);
				break;

			case SceneComponent::HITBOXES :
			{
				for (Physics::HitBox* hb : *(std::vector<Physics::HitBox*>*)&component)
				{
					if (hb->enable_collision)
						Physics::HitBox::s_colliders[id].push_back(hb);
					for (auto& visualliser : hb->visuallisers)
						Physics::HitBox::visuallisers_layers[id].meshes.push_back(&visualliser);
				}
				Physics::HitBox::visuallisers_layers[id].enable_ptr = &Physics::HitBox::enable_visuallisers;
				auto it = std::find(layers.begin(), layers.end(), &Physics::HitBox::visuallisers_layers[id]);
				if (it == layers.end())
					layers.push_back(&Physics::HitBox::visuallisers_layers[id]);
				break;
			}

			case SceneComponent::PHYS_VECS :
				for (void* vec : component)
					physics_vecs.push_back(reinterpret_cast<Physics::PhysicsVecs*>(vec));
				break;

			case SceneComponent::PHYS_ENTITIES :
			{
				for (Physics::PhysicsEntity* entity : *(std::vector<Physics::PhysicsEntity*>*)& component)
				{
					if (entity->enable_collision)
						Physics::HitBox::s_colliders[id].push_back(entity);
					for (auto& visualliser : entity->visuallisers)
						Physics::HitBox::visuallisers_layers[id].meshes.push_back(&visualliser);
					physics_vecs.push_back(entity);
				}
				Physics::HitBox::visuallisers_layers[id].enable_ptr = &Physics::HitBox::enable_visuallisers;
				auto it = std::find(layers.begin(), layers.end(), &Physics::HitBox::visuallisers_layers[id]);
				if (it == layers.end())
					layers.push_back(&Physics::HitBox::visuallisers_layers[id]);
				break;
			}

			default:
				break;
			}
		}

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

	void Scene::switch_scene(int new_scene_id)
	{
		if (scenes.find(new_scene_id) == scenes.end())
			return;

		if (std::this_thread::get_id() == Renderer::thread_id)
		{
			scenes[scene_id]->unload();
			scenes[new_scene_id]->load();
			for (auto& layer : Renderer::global_layers)
				layer->load();
			scene_id = new_scene_id;
		}
		else
		{
			Renderer::pending_actions.push_back([new_scene_id]()
				{
					scenes[scene_id]->unload();
					scenes[new_scene_id]->load();
					for (auto& layer : Renderer::global_layers)
						layer->load();
					scene_id = new_scene_id;
				});
		}
	}

}