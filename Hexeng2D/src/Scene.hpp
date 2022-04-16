#ifndef SCENE_HPP
#define SCENE_HPP

#include "Macros.hpp"
#include "Renderer/Layer.hpp"
#include "EventManager/EventManager.hpp"
#include "Physics/Physics.hpp"

namespace Hexeng
{

	enum class HXG_DECLSPEC SceneComponent
	{
		LAYERS,
		CONTEXTUAL_LAYERS,
		EVENTS,
		HITBOXES,
		PHYS_VECS,
		PHYS_ENTITIES
	};

	class HXG_DECLSPEC Scene
	{

	public:

		std::vector<Renderer::Layer*> layers;
		std::vector<Renderer::ContextualLayer*> contextual_layers;
		std::vector<std::pair<EventManager::Event*, unsigned int>> events;
		std::vector<Physics::PhysicsVecs*> physics_vecs;

		Scene(int id, const std::vector<std::pair<SceneComponent, std::vector<void*>>>& components);

		void load();
		void unload();

		static void switch_scene(int new_scene_id);
	};

	HXG_DECLSPEC extern std::unordered_map<int, Scene*> scenes;
	HXG_DECLSPEC extern int scene_id;

}

#endif // !SCENE_HPP