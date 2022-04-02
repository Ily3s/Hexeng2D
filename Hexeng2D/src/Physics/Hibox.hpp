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
		Vec2<int> prev_min, prev_max;
		Vec2<int> size;

		RectangleHitBox(Vec2<int> min, Vec2<int> max) : min(min), max(max), size(max - min), prev_min(min), prev_max(max) {}
	};

	class HXG_DECLSPEC HitBox
	{
	protected:

		std::vector<RectangleHitBox> m_rectangles;
		int m_solidity;

		static std::vector<HitBox*> s_colliders;

	public:

		HitBox(const std::vector<RectangleHitBox>& rectangles, int solidity, bool enable_collision = true);

		static EventManager::Event collisions_evt;
		static void load_collisions();
		static std::pair<RectangleHitBox*, RectangleHitBox*> is_colliding(HitBox& hb1, HitBox& hb2);
		static bool is_colliding(const RectangleHitBox& hb1, const RectangleHitBox& hb2);

		virtual void on_collision(std::pair<RectangleHitBox*, RectangleHitBox*>) {};
	};
}

#endif