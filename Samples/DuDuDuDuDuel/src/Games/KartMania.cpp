#include "../GameSelection.hpp"
#include "../Menu.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace KartMania
{
	Hexeng::Renderer::Layer game_layer;

	Hexeng::Renderer::Texture kart_tex;

	class Kart
	{
	public:

		Kart() = default;

		Kart& operator=(Kart&&) noexcept;

		Kart(Hexeng::Vec2<int> pos, double rotation, Hexeng::Color4 color, int up, int left, int down, int right);

		Hexeng::Renderer::Quad mesh;
		Hexeng::Physics::PhysicsEntity physics{ {}, 0 };

		Hexeng::EventManager::KeyEvent up, down;
		Hexeng::EventManager::KeyPressEvent left_press, right_press, left_release, right_release;

		Hexeng::EventManager::EventGate update_velocity;

		float velocity_norm = 0, previous_velocity_norm = 0;
		double physics_angle_der = 0, physics_angle = 0;

		int up_key, left_key, down_key, right_key;
	}
	kart1, kart2;

	Hexeng::Scene kartmania_scene{ 8, {
		{Hexeng::SceneComponent::LAYERS, {&game_layer, &ig_escape_layer, &ig_win_layer}},
		{Hexeng::SceneComponent::EVENTS, {&kart1.up, &kart1.down, &kart2.up, &kart2.down, &kart1.update_velocity, &kart2.update_velocity}},
		{Hexeng::SceneComponent::PHYS_ENTITIES, {&kart1.physics, &kart2.physics}}
	 } };

	Hexeng::Renderer::ToBeInit init_km
	{
		[]()
		{
			for (auto evt : ig_base_evts)
				kartmania_scene.events.push_back(evt);

			kart_tex = Hexeng::Renderer::Texture{ "res/games/kartmania/kart.png", {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };

			kart1 = Kart({ -500, 0 }, 360 - 90, Hexeng::Color4{ 0.5f, 1.0f, 1.0f, 1.0f }, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D);
			kart2 = Kart({ 500, 0 }, 90, Hexeng::Color4{ 1.0f, 0.5f, 0.5f, 1.0f }, GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_RIGHT);

			game_layer = Hexeng::Renderer::Layer{ {&background, &kart1.mesh, &kart2.mesh}, 0, Hexeng::Renderer::Position::ABSOLUTE };
		}
	, 9};

	Kart& Kart::operator=(Kart&& other) noexcept
	{
		mesh = std::move(other.mesh);
		up_key = other.up_key, down_key = other.down_key, left_key = other.left_key, right_key = other.right_key;

		physics = std::move(other.physics);
		physics.link(mesh);

		up = { up_key, [this]() { velocity_norm += 0.3f; }, Hexeng::Range::LOCAL };
		down = { down_key, [this]() { velocity_norm -= 0.15f; }, Hexeng::Range::LOCAL };
			left_press = {left_key, [this]() {if (Hexeng::scene_id == 8) {
			physics_angle_der += 30;
		} }, GLFW_PRESS };
		right_press = { right_key, [this]() {if (Hexeng::scene_id == 8) {
			physics_angle_der -= 30;
	}}, GLFW_PRESS };
		left_release = { left_key, [this]() {if (Hexeng::scene_id == 8) {
		physics_angle_der -= 30;
	}}, GLFW_RELEASE };
		right_release = { right_key, [this]() {if (Hexeng::scene_id == 8) {
		physics_angle_der += 30;
	}}, GLFW_RELEASE };
		int rota = mesh.rotation;
		update_velocity = { [this, rota]() {
		if (physics_angle_der != 0)
		{
			previous_velocity_norm *= 0.9f;
			physics.velocity *= 0.9f;
		}
		physics_angle += velocity_norm * 0.005f * physics_angle_der;
		mesh.rotation = -physics_angle + rota;
		float velocity_variation = velocity_norm - previous_velocity_norm;
		physics.velocity += Hexeng::Vec2<float>{(float)std::cos((-rota - 90 + physics_angle + physics_angle_der)* M_PI / 180)* velocity_variation, (float)std::sin((-rota - 90 + physics_angle + physics_angle_der)* M_PI / 180)* velocity_variation};
		physics.velocity *= 0.99f;
		velocity_norm *= 0.95f;
		previous_velocity_norm = velocity_norm;
			}, Hexeng::Range::LOCAL };
		
		return *this;
	}

	Kart::Kart(Hexeng::Vec2<int> pos, double rotation, Hexeng::Color4 color, int up_keyp, int left, int down_keyp, int right)
		: mesh(pos, 5.0f, &kart_tex),
		up_key(up_keyp), left_key(left), down_key(down_keyp), right_key(right),
		physics({ {{- kart_tex.get_size()*3.0f / 2}, {kart_tex.get_size() *3.0f/ 2}}}, 0)
	{
		mesh.color_filter = color;
		mesh.rotation = rotation;
		physics.move(pos);
	}

}