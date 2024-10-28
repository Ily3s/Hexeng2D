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
		EVENTS,
		HITBOXES,
		PHYS_VECS,
		PHYS_ENTITIES
	};

	class HXG_DECLSPEC Scene
	{

	public:

		std::vector<Renderer::Layer*> layers;
		std::vector<EventManager::Event*> events;
		std::vector<Physics::PhysicsVecs*> physics_vecs;

		Scene(int id, const std::unordered_map<SceneComponent, std::vector<void*>>& components = {});

		void load();
		void unload();

		static void switch_scene(int new_scene_id);
	};

	HXG_DECLSPEC extern std::unordered_map<int, Scene*> scenes;

	/// @note You shouldn't modify that var during the game loop, instead, call switch_scene
	HXG_DECLSPEC extern int scene_id;

}

#endif // !SCENE_HPP