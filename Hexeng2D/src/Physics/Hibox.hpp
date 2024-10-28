#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <vector>
#include <unordered_map>

#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Renderer/Layer.hpp"
#include "../Renderer/Quad.hpp"

namespace Hexeng
{
	class Scene;
}

namespace Hexeng::Physics
{
	struct HXG_DECLSPEC RectangleHitBox
	{
		Vec2<int> min, max;
		Vec2<int> prev_min, prev_max;
		Vec2<int> size;

		RectangleHitBox(Vec2<int> min, Vec2<int> max) : min(min), max(max), size(max - min), prev_min(min), prev_max(max) {}
	};

	enum class From
	{
		NOWHERE = 0,
		BOT,
		TOP,
		LEFT,
		RIGHT
	};

	class HXG_DECLSPEC HitBox
	{
	protected:

		friend class Hexeng::Scene;

		std::vector<RectangleHitBox> m_rectangles;
		int m_solidity = 0;
		bool enable_collision = false;

		static std::unordered_map<int, std::vector<HitBox*>> s_colliders;

		static std::unordered_map <int, Renderer::Layer> visuallisers_layers;
		std::vector<Renderer::DebugQuad> visuallisers;

	public:

		HitBox(const std::vector<RectangleHitBox>& rectangles, int solidity, bool enable_collision = true);

		HitBox(const HitBox&) = delete;
		HitBox& operator=(const HitBox&) = delete;

		HitBox(HitBox&&) noexcept;
		HitBox& operator=(HitBox&&) noexcept;

		HitBox() = default;

		static EventManager::EventGate collisions_evt;
		static void load_collisions();
		static std::pair<RectangleHitBox*, RectangleHitBox*> is_colliding(HitBox& hb1, HitBox& hb2);
		static bool is_colliding(const RectangleHitBox& hb1, const RectangleHitBox& hb2);

		std::vector<RectangleHitBox> view_prev_state();
		std::vector<RectangleHitBox> view_prev_state_x();
		std::vector<RectangleHitBox> view_prev_state_y();

		static From where_colliding(HitBox& hb1, HitBox& hb2);

		virtual void on_collision(std::pair<RectangleHitBox*, RectangleHitBox*>) {};

		static void set_visuallisers_z(int z_pos);

		static bool enable_visuallisers;
	};
}

#endif