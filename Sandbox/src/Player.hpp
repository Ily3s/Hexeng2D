#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Mesh.hpp"
#include "Physics/Physics.hpp"
#include "EventManager/EventManager.hpp"
#include "Vectors.hpp"

using namespace Hexeng;

class Player
{
private:

	static Player* instance;

public:

	Renderer::Quad quad;
	Renderer::SuperMesh mesh;
	Physics::PhysicsEntity physics;
	int speed = 4;

	Player(Vec2<int> pos, float size, Renderer::Texture* tex);
};

Player* Player::instance = nullptr;

Player::Player(Vec2<int> pos, float size, Renderer::Texture* tex)
	: quad({ 0, 0 }, size, tex, true),
	mesh({ &quad }),
	physics({ { -tex->get_size()*size / 2, tex->get_size()*size / 2} }, 1)
{
	physics.position = pos;
	physics.link(mesh);

	if (instance)
		return;

	instance = this;

	static EventManager::Event follow_cam = { []() {return true; }, [this]()
		{Renderer::Camera::position.x = physics.position.x; Renderer::Camera::position.y = physics.position.y; Renderer::Camera::refresh_pos(); } };
}

#endif
