#include "Physics.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Renderer/Renderer.hpp"

namespace Hexeng::Physics
{
	PhysicsEntity::PhysicsEntity(std::vector<RectangleHitBox> rectangles, int solidity)
		: HitBox(rectangles, solidity, true),
		PhysicsVecs()
	{
		s_entities.push_back(this);

		for (auto& visualliser : visuallisers)
			visualliser.uniforms.push_back({ &Renderer::u_transform, &transform });
	}

	void PhysicsEntity::on_collision(std::pair<RectangleHitBox*, RectangleHitBox*> collisioners)
	{
		collisioners.first->min.x = collisioners.first->prev_min.x;
		collisioners.first->max.x = collisioners.first->prev_max.x;

		Vec2<int> bckp = { collisioners.second->min.x, collisioners.second->max.x };

		collisioners.second->min.x = collisioners.second->prev_min.x;
		collisioners.second->max.x = collisioners.second->prev_max.x;

		bool is_still_collisioning = is_colliding(*collisioners.first, *collisioners.second);

		collisioners.first->min.x += instant_speed.x;
		collisioners.first->max.x += instant_speed.x;

		collisioners.second->min.x = bckp.x;
		collisioners.second->max.x = bckp.y;

		if (!is_still_collisioning)
		{
			if (collisioners.first->prev_min.x < collisioners.second->prev_min.x)
				position.x += collisioners.second->min.x - collisioners.first->max.x;
			else
				position.x += collisioners.second->max.x - collisioners.first->min.x;

			velocity.x = 0;
			instant_speed.x = 0;
		}
		else
		{
			if (collisioners.first->prev_min.y < collisioners.second->prev_min.y)
				position.y += collisioners.second->min.y - collisioners.first->max.y;
			else
				position.y += collisioners.second->max.y - collisioners.first->min.y;

			velocity.y = 0;
			instant_speed.y = 0;
		}

		update_rec_position(*collisioners.first);
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
			update_rec_position(rec);
	}

	void PhysicsEntity::update_rec_position(RectangleHitBox& rec)
	{
		rec.prev_max = rec.max;
		rec.prev_min = rec.min;
		rec.min = position - rec.size / 2;
		rec.max = position + rec.size / 2;
	}

	PhysicsEntity::PhysicsEntity(PhysicsEntity&& other) noexcept
		: HitBox(std::move(other)), PhysicsVecs(std::move(other))
	{
		auto it = std::find(s_entities.begin(), s_entities.end(), &other);
		if (it != s_entities.end())
			*it = this;
	}

	PhysicsEntity& PhysicsEntity::operator=(PhysicsEntity&& other) noexcept
	{
		HitBox::operator=(std::move(other));
		PhysicsVecs::operator=(std::move(other));

		auto it = std::find(s_entities.begin(), s_entities.end(), this);
		if (it != s_entities.end())
			s_entities.erase(it);

		it = std::find(s_entities.begin(), s_entities.end(), &other);
		if (it != s_entities.end())
			*it = this;

		return *this;
	}

	EventManager::EventGate PhysicsEntity::update_position_evt	{ update_positions_all };
	EventManager::EventGate HitBox::collisions_evt				{ load_collisions };
	EventManager::EventGate PhysicsVecs::update_evt				{ update_vecs };

}