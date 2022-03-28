#include "Mouvement.hpp"
#include "../Functions.hpp"
#include "../Renderer/Renderer.hpp"

namespace Hexeng::Physics
{
	std::vector<PhysicsVecs*> PhysicsVecs::s_vecs;

	Vec2<float> PhysicsVecs::acceleration{ 0.0f, 0.0f };

	PhysicsVecs::PhysicsVecs()
	{
		s_vecs.push_back(this);
	}

	void PhysicsVecs::link(Renderer::Mesh& mesh)
	{
		mesh.uniforms.push_back({ &Renderer::u_transform, &transform });
	}

	EventManager::Event PhysicsVecs::update_evt{ []() {return true; }, update_vecs };

	void PhysicsVecs::update()
	{
		velocity += acceleration;
		position.x += velocity.x;
		position.y += velocity.y;
		transform = toCoord(position);
	}

	void PhysicsVecs::update_vecs()
	{
		for (PhysicsVecs* vec : s_vecs)
			vec->update();
	}

}