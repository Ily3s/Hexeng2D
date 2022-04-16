#ifndef MOUVEMENT_HPP
#define MOUVEMENT_HPP

#include <vector>

#include "../EventManager/EventManager.hpp"
#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "../Renderer/Mesh.hpp"

namespace Hexeng::Physics
{

	class HXG_DECLSPEC PhysicsVecs
	{
	private :

		uint64_t m_last_tick = 0;

		Renderer::Mesh* m_mesh = nullptr;

	public:

		Vec2<float> transform{ 0.0f, 0.0f };
		Vec2<int> position{ 0, 0 };
		Vec2<int> instant_speed{ 0, 0 };
		Vec2<float> velocity{ 0, 0 };

		void move(Vec2<int> offset);

		static Vec2<float> acceleration;

		void link(Renderer::Mesh& mesh);

		void update();
		static void update_vecs();
		static EventManager::EventGate update_evt;
	};

}

#endif