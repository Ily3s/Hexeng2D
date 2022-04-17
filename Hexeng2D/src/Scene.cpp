#include <algorithm>

#include "Scene.hpp"
#include "Renderer/Renderer.hpp"

namespace Hexeng
{

	std::unordered_map<int, Scene*> scenes;
	int scene_id = 0;

	Scene::Scene(int id, const std::vector<std::pair<SceneComponent, std::vector<void*>>>& components)
	{
		for (auto& [type, component] : components)
		{
			switch (type)
			{
			case SceneComponent::LAYERS :
				layers = *(std::vector<Renderer::Layer*>*)&component;
				std::sort(layers.begin(), layers.end(), [](Renderer::Layer* layer1, Renderer::Layer* layer2) {
					return layer1->z_position > layer2->z_position; });
				break;

			case SceneComponent::CONTEXTUAL_LAYERS :
				contextual_layers = *(std::vector<Renderer::ContextualLayer*>*)&component;
				std::sort(contextual_layers.begin(), contextual_layers.end(), [](Renderer::ContextualLayer* layer1, Renderer::ContextualLayer* layer2) {
					return layer1->z_position > layer2->z_position; });
				break;

			case SceneComponent::EVENTS :
				for (EventManager::Event* evt : *(std::vector<EventManager::Event*>*)&component)
					events.emplace_back(evt, evt->pertick);
				break;

			case SceneComponent::HITBOXES :
			{
				for (Physics::HitBox* hb : *(std::vector<Physics::HitBox*>*)& component)
				{
					if (hb->enable_collision)
						Physics::HitBox::s_colliders[id].push_back(hb);
					for (auto& visualliser : hb->visuallisers)
						Physics::HitBox::visuallisers_layers[id].meshes.push_back(&visualliser);
				}
				Physics::HitBox::visuallisers_layers[id].context = &Physics::HitBox::enable_visuallisers;
				auto it = std::find(contextual_layers.begin(), contextual_layers.end(), &Physics::HitBox::visuallisers_layers[id]);
				if (it == contextual_layers.end())
					contextual_layers.push_back(&Physics::HitBox::visuallisers_layers[id]);
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
				Physics::HitBox::visuallisers_layers[id].context = &Physics::HitBox::enable_visuallisers;
				auto it = std::find(contextual_layers.begin(), contextual_layers.end(), &Physics::HitBox::visuallisers_layers[id]);
				if (it == contextual_layers.end())
					contextual_layers.push_back(&Physics::HitBox::visuallisers_layers[id]);
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

		for (auto cl : contextual_layers)
			cl->load();
	}

	void Scene::unload()
	{
		for (auto layer : layers)
			layer->unload();

		for (auto cl : contextual_layers)
			cl->unload();
	}

	void Scene::switch_scene(int new_scene_id)
	{
		if (scenes.find(new_scene_id) == scenes.end())
			return;

		Renderer::pending_actions.push_back([new_scene_id]()
			{
				scenes[scene_id]->unload();
				scenes[new_scene_id]->load();
				for (auto& layer : Renderer::global_layers)
					layer->load();
				for (auto& cl : Renderer::global_contextual_layers)
					cl->load();
				scene_id = new_scene_id;
			});
	}

}