#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <vector>

#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "../EventManager/EventManager.hpp"

namespace Hexeng::Physics
{
	struct HXG_DECLSPEC RectangleHitBox
	{
		Vec2<int> min, max;
	};

	class HXG_DECLSPEC HitBox
	{
	protected :

		std::vector<RectangleHitBox> m_rectangles;

		static std::vector<HitBox*> s_colliders;

	public :

		HitBox(const std::vector<RectangleHitBox>& rectangles, bool enable_collision = true);

		static EventManager::Event collisions_evt;
		static void load_collisions();
		static int is_colliding(const HitBox& hb1, const HitBox& hb2);
		static int is_colliding(const RectangleHitBox& hb1, const RectangleHitBox& hb2);

		virtual void on_collision(int x_or_y) {};
	};
}

#endif