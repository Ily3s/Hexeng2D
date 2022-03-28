#include "Physics.hpp"
#include "../EventManager/EventManager.hpp"

namespace Hexeng::Physics
{
	PhysicsEntity::PhysicsEntity(std::vector<RectangleHitBox> rectangles, bool is_solid)
		: HitBox(rectangles, true), m_is_solid(is_solid) {}

	void PhysicsEntity::on_collision(int x_or_y)
	{
		if (m_is_solid)
			return;

		if (x_or_y == 1)
		{
			position.x -= velocity.x;
			velocity.x = 0;
		}
		else
		{
			position.y -= velocity.y;
			velocity.y = 0;
		}
	}
}