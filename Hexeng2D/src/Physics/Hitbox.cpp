#include "Hibox.hpp"
#include "Hexeng.hpp"
#include "Scene.hpp"

namespace Hexeng::Physics
{
	std::unordered_map<int, std::vector<HitBox*>> HitBox::s_colliders;

	std::unordered_map <int, Renderer::Layer> HitBox::visuallisers_layers;

	HitBox::HitBox(const std::vector<RectangleHitBox>& rectangles, int solidity, bool enable_collision)
		: m_rectangles(rectangles),
		m_solidity(solidity),
		enable_collision(enable_collision)
	{
		for (const auto& rec : rectangles)
		{
			visuallisers.emplace_back(rec.min + rec.size/2, rec.size);
			if (enable_collision)
				visuallisers.back().uniforms.push_back({ &Renderer::u_color, &Color4::green });
			else
				visuallisers.back().uniforms.push_back({ &Renderer::u_color, &Color4::blue });
		}
	}

	HitBox::HitBox(HitBox&& other) noexcept
		: m_rectangles(std::move(other.m_rectangles)),
		m_solidity(other.m_solidity),
		visuallisers(std::move(other.visuallisers)),
		enable_collision(other.enable_collision)
	{
		for (auto& [i, vec] : s_colliders)
		{
			auto it = std::find(vec.begin(), vec.end(), &other);
			if (it != vec.end())
				*it = this;
		}
	}

	HitBox& HitBox::operator=(HitBox&& other) noexcept
	{
		m_rectangles = std::move(other.m_rectangles);
		m_solidity = other.m_solidity;
		visuallisers = std::move(other.visuallisers);
		enable_collision = other.enable_collision;

		for (auto& [i, vec] : s_colliders)
		{
			auto it = std::find(vec.begin(), vec.end(), &other);
			if (it != vec.end())
				vec.erase(it);
		}

		for (auto& [i, vec] : s_colliders)
		{
			auto it = std::find(vec.begin(), vec.end(), &other);
			if (it != vec.end())
				*it = this;
		}

		return *this;
	}

	void HitBox::load_collisions()
	{
		if (s_colliders[scene_id].size() > 1)
		{
			for (int i1 = 0; i1 < s_colliders[scene_id].size() - 1; i1++)
			{
				HitBox& hb1 = *s_colliders[scene_id][i1];
				for (int i2 = i1 + 1; i2 < s_colliders[scene_id].size(); i2++)
				{
					HitBox& hb2 = *s_colliders[scene_id][i2];
					std::pair<RectangleHitBox*, RectangleHitBox*> temp = is_colliding(hb1, hb2);
					if (temp.first)
					{
						if (hb1.m_solidity > hb2.m_solidity)
							hb2.on_collision({ temp.second, temp.first });
						else
							hb1.on_collision(temp);
					}
				}
			}
		}
	}

	std::pair<RectangleHitBox*, RectangleHitBox*> HitBox::is_colliding(HitBox& hb1, HitBox& hb2)
	{
		for (auto& rec1 : hb1.m_rectangles)
		{
			for (auto& rec2 : hb2.m_rectangles)
			{
				if (int temp = is_colliding(rec1, rec2))
					return { &rec1, &rec2 };
			}
		}
		return { nullptr, nullptr };
	}

	bool HitBox::is_colliding(const RectangleHitBox& hb1, const RectangleHitBox& hb2)
	{
		bool cond1x = hb1.min.x > hb2.min.x && hb1.min.x < hb2.max.x;
		bool cond2x = hb1.max.x > hb2.min.x && hb1.max.x < hb2.max.x;
		bool cond3x = hb2.min.x > hb1.min.x && hb2.min.x < hb1.max.x;
		bool cond4x = hb2.max.x > hb1.min.x && hb2.max.x < hb1.max.x;

		bool condx = cond1x || cond2x || cond3x || cond4x;

		bool cond1y = hb1.min.y > hb2.min.y && hb1.min.y < hb2.max.y;
		bool cond2y = hb1.max.y > hb2.min.y && hb1.max.y < hb2.max.y;
		bool cond3y = hb2.min.y > hb1.min.y && hb2.min.y < hb1.max.y;
		bool cond4y = hb2.max.y > hb1.min.y && hb2.max.y < hb1.max.y;

		bool condy = cond1y || cond2y || cond3y || cond4y;

		return condx && condy;
	}

	From HitBox::where_colliding(HitBox& hb1, HitBox& hb2)
	{
		auto collision_pair = is_colliding(hb1, hb2);

		if (!collision_pair.first)
			return From::NOWHERE;

		auto bckp1 = hb1.view_prev_state();
		auto bckp2 = hb2.view_prev_state();

		if (is_colliding(hb1, hb2).first)
			return From::NOWHERE;

		hb1.m_rectangles = bckp1;
		hb2.m_rectangles = bckp2;

		bckp1 = hb1.view_prev_state_x();
		bckp2 = hb2.view_prev_state_x();

		bool collision_in_x = !is_colliding(hb1, hb2).first;

		hb1.m_rectangles = bckp1;
		hb2.m_rectangles = bckp2;

		if (collision_in_x)
			return collision_pair.first->min.x < collision_pair.second->min.x ? From::LEFT : From::RIGHT;
		else
			return collision_pair.first->min.y < collision_pair.second->min.y ? From::BOT : From::TOP;
	}

	std::vector<RectangleHitBox> HitBox::view_prev_state()
	{
		std::vector<RectangleHitBox> bckp = m_rectangles;

		for (auto& rec : m_rectangles)
		{
			rec.min = rec.prev_min;
			rec.max = rec.prev_max;
		}

		return bckp;
	}

	std::vector<RectangleHitBox> HitBox::view_prev_state_x()
	{
		std::vector<RectangleHitBox> bckp = m_rectangles;

		for (auto& rec : m_rectangles)
		{
			rec.min.x = rec.prev_min.x;
			rec.max.x = rec.prev_max.x;
		}

		return bckp;
	}

	std::vector<RectangleHitBox> HitBox::view_prev_state_y()
	{
		std::vector<RectangleHitBox> bckp = m_rectangles;

		for (auto& rec : m_rectangles)
		{
			rec.min.x = rec.prev_min.x;
			rec.max.x = rec.prev_max.x;
		}

		return bckp;
	}

	void HitBox::set_visuallisers_z(int z_pos)
	{
		for (auto& [scene, layer] : visuallisers_layers)
			layer.z_position = z_pos;
	}

	bool HitBox::enable_visuallisers = false;

}