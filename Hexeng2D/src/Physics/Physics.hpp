#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../Macros.hpp"
#include "Hibox.hpp"
#include "Mouvement.hpp"
#include "Renderer/Mesh.hpp"

namespace Hexeng::Physics
{

	class HXG_DECLSPEC PhysicsEntity : public HitBox, public PhysicsVecs
	{
	private :

		bool m_is_solid;

		static std::vector<PhysicsEntity*> s_entities;
		static void update_positions_all();
		void update_positions();
		static EventManager::Event update_position_evt;

	public :

		PhysicsEntity(std::vector<RectangleHitBox> rectangles, bool is_solid = false);

		void on_collision(std::pair<RectangleHitBox*, RectangleHitBox*>) override;

	};

}

#endif