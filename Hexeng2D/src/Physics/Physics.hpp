#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../Macros.hpp"
#include "Hibox.hpp"
#include "Mouvement.hpp"
#include "Renderer/Mesh.hpp"

namespace Hexeng::Physics
{

	class HXG_DECLSPEC PhysicsEntity : public HitBox, PhysicsVecs
	{
	private :

		bool m_is_solid;

	public :

		PhysicsEntity(std::vector<RectangleHitBox> rectangles, bool is_solid = false);

		void on_collision(int x_or_y) override;
	};

}

#endif