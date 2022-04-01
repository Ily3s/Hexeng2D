#include <thread>
#include <chrono>

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Presets/BasicQuad.hpp"
#include "Renderer/Layer.hpp"
#include "Renderer/Scene.hpp"
#include "Renderer/Camera.hpp"
#include "EventManager/EventManager.hpp"
#include "EventManager/InputEvent.hpp"
#include "Physics/Physics.hpp"

#include "Player.hpp"

int main()
{
	using namespace Hexeng;

	Settings::window_name = "Sandbox";

	Renderer::init();

	Renderer::Texture example{ "res/example.png", GL_NEAREST };
	Renderer::Presets::BasicSquare square{ { 0, 0 }, 5.0f, &example, true };
	Renderer::Presets::BasicSquare square2{ { -100, -100 }, 35.0f, &example, true };
	Renderer::Presets::BasicSquare square3{ { 2000, -250 }, 500, &example };

	Player player{{0, 0 }, 5.0f, & example};

	Renderer::Layer fore_ground{ {&player.mesh, &square3}, 750 };
	Renderer::Layer back_ground1{ {&square}, 500 };
	Renderer::Layer back_ground2{ {&square2}, 800 };
	Renderer::Scene first_scene{ 1, { &fore_ground , &back_ground1, &back_ground2 } };

	Renderer::scene_id = 1;

	EventManager::KeyEvent go_up{ 87, [&player]() {player.physics.move({0, 4}); } };
	EventManager::KeyEvent go_left{ 65, [&player]() {player.physics.move({-4, 0}); } };
	EventManager::KeyEvent go_down{ 83, [&player]() {player.physics.move({0, -4});} };
	EventManager::KeyEvent go_right{ 68, [&player]() {player.physics.move({4, 0});} };

	EventManager::ScrollEvent::get()->callback = [](double amount) {Renderer::Camera::position.z += amount * 10; Renderer::Camera::refresh_pos(); };

	Physics::HitBox hb = Physics::HitBox({ {{2000, -250}, {2500, 250}} });

	EventManager::start_looping();

	Hexeng::game_loop([]()
		{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(16ms);
		});

	EventManager::stop_looping();

	Renderer::stop();

	return 0;
}