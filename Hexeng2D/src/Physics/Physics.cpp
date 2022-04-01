#include "Physics.hpp"
#include "../EventManager/EventManager.hpp"

namespace Hexeng::Physics
{
	PhysicsEntity::PhysicsEntity(std::vector<RectangleHitBox> rectangles, bool is_solid)
		: HitBox(rectangles, true), m_is_solid(is_solid)
	{
		s_entities.push_back(this);
	}

	void PhysicsEntity::on_collision(std::pair<RectangleHitBox*, RectangleHitBox*> collisioners)
	{
		if (m_is_solid)
			return;

		collisioners.first->min.x -= instant_speed.x;
		collisioners.first->max.x -= instant_speed.x;

		if (!is_colliding(*collisioners.first, *collisioners.second))
		{
			collisioners.first->min.x += instant_speed.x;
			collisioners.first->max.x += instant_speed.x;
			position.x -= instant_speed.x;
			velocity.x = 0;
			instant_speed.x = 0;
		}
		else
		{
			collisioners.first->min.x += instant_speed.x;
			collisioners.first->max.x += instant_speed.x;
			position.y -= instant_speed.y;
			velocity.y = 0;
			instant_speed.y = 0;
		}
	}

	std::vector<PhysicsEntity*> PhysicsEntity::s_entities;

	void PhysicsEntity::update_positions_all()
	{
		for (auto& entity : s_entities)
			entity->update_positions();
	}

	void PhysicsEntity::update_positions()
	{
		for (auto& rec : m_rectangles)
		{
			rec.min = position - rec.size / 2;
			rec.max = position + rec.size / 2;
		}
	}

	EventManager::Event PhysicsEntity::update_position_evt{ []() {return true; }, update_positions_all };
	EventManager::Event HitBox::collisions_evt{ []() {return true; }, load_collisions };
	EventManager::Event PhysicsVecs::update_evt{ []() {return true; }, update_vecs };

}