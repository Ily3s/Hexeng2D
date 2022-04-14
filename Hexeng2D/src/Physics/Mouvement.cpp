#include "Mouvement.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Hexeng.hpp"

namespace Hexeng::Physics
{
	std::vector<PhysicsVecs*> PhysicsVecs::s_vecs;

	Vec2<float> PhysicsVecs::acceleration{ 0.0f, 0.0f };

	PhysicsVecs::PhysicsVecs()
	{
		s_vecs.push_back(this);
	}

	void PhysicsVecs::move(Vec2<int> offset)
	{
		position += offset;
		if (EventManager::current_tick != m_last_tick)
			instant_speed = offset;
		else
			instant_speed += offset;

		m_last_tick = EventManager::current_tick;
	}

	void PhysicsVecs::update()
	{
		velocity += acceleration;
		move({ (int)velocity.x, (int)velocity.y });
		if (m_mesh)
			m_mesh->position = position;
		transform = toCoord(position);
	}

	void PhysicsVecs::link(Renderer::Mesh& mesh)
	{
		m_mesh = &mesh;
	}

	void PhysicsVecs::update_vecs()
	{
		for (PhysicsVecs* vec : s_vecs)
			vec->update();
	}

}