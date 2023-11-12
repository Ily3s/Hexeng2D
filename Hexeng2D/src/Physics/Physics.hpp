#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../Macros.hpp"
#include "Hibox.hpp"
#include "Mouvement.hpp"
#include "Renderer/Mesh.hpp"

/**
* @brief I'm sorry, this Physics Engine is so messy,
* even I have trouble reading through this, and I am the one that wrote it.
* No, really I can't possibly document that.
* A rework is coming, but, in the meantime,
* I recommand that you take a look at the Sandbox project in order for you to understand the basics of what I dared to call a Physics Engine.
* */

namespace Hexeng::Physics
{

	class HXG_DECLSPEC PhysicsEntity : public HitBox, public PhysicsVecs
	{
	private :

		static std::vector<PhysicsEntity*> s_entities;
		static void update_positions_all();
		void update_positions();
		void update_rec_position(RectangleHitBox& rec);
		static EventManager::EventGate update_position_evt;

	public :

		PhysicsEntity(std::vector<RectangleHitBox> rectangles, int solidity);

		void on_collision(std::pair<RectangleHitBox*, RectangleHitBox*>) override;

		PhysicsEntity(PhysicsEntity&& other) noexcept;
		PhysicsEntity& operator=(PhysicsEntity&& other) noexcept;
	};

}

#endif