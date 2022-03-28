#include "Hibox.hpp"

namespace Hexeng::Physics
{
	std::vector<HitBox*> HitBox::s_colliders;

	HitBox::HitBox(const std::vector<RectangleHitBox>& rectangles, bool enable_collision)
		: m_rectangles(rectangles)
	{
		if (enable_collision)
			s_colliders.push_back(this);
	}

	EventManager::Event HitBox::collisions_evt{ []() {return true; }, load_collisions };

	void HitBox::load_collisions()
	{
		if (s_colliders.size() > 1)
		{
			for (int i1 = 0; i1 < s_colliders.size(); i1++)
			{
				HitBox& hb1 = *s_colliders[i1];
				for (int i2 = i1+1; i2 < s_colliders.size(); i2++)
				{
					HitBox& hb2 = *s_colliders[i2];
					if (int temp = is_colliding(hb1, hb2))
					{
						hb1.on_collision(temp);
						hb2.on_collision(temp);
					}
				}
			}
		}
	}

	int HitBox::is_colliding(const HitBox& hb1, const HitBox& hb2)
	{
		for (const auto& rec1 : hb1.m_rectangles)
		{
			for (const auto& rec2 : hb2.m_rectangles)
			{
				if (int temp = is_colliding(rec1, rec2))
					return temp;
			}
		}
		return 0;
	}

	int HitBox::is_colliding(const RectangleHitBox& hb1, const RectangleHitBox& hb2)
	{
		bool cond1x = hb1.min.x > hb2.min.x && hb1.min.x < hb2.max.x;
		bool cond2x = hb1.max.x > hb2.min.x && hb1.max.x < hb2.max.x;
		bool cond3x = hb1.min.x < hb2.min.x && hb1.max.x > hb2.max.x;

		bool condx = cond1x || cond2x || cond3x;

		bool cond1y = hb1.min.y > hb2.min.y && hb1.min.y < hb2.max.y;
		bool cond2y = hb1.max.y > hb2.min.y && hb1.max.y < hb2.max.y;
		bool cond3y = hb1.min.y < hb2.min.y&& hb1.max.y > hb2.max.y;

		bool condy = cond1y || cond2y || cond3y;

		return condx ? 1 : condy ? 2 : 0;
	}
}