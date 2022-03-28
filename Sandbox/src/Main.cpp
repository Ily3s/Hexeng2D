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

int main()
{
	using namespace Hexeng;

	Settings::window_name = "Sandbox";

	Renderer::init();

	Renderer::Texture example{ "res/example.png", GL_NEAREST };
	Renderer::Presets::BasicSquare square{ { 0, 0 }, 5.0f, &example, true };
	Renderer::Presets::BasicSquare square2{ { -100, -100 }, 35.0f, &example, true };

	Renderer::Layer fore_ground{ {&square}, 500 };
	Renderer::Layer back_ground{ {&square2}, 1000 };
	Renderer::Scene first_scene{ 1, { &fore_ground , &back_ground } };

	Renderer::scene_id = 1;

	EventManager::KeyEvent go_up{ 87, []() {Renderer::Camera::position.y += 2; Renderer::Camera::refresh_pos(); } };
	EventManager::KeyEvent go_left{ 65, []() {Renderer::Camera::position.x -= 2; Renderer::Camera::refresh_pos(); } };
	EventManager::KeyEvent go_down{ 83, []() {Renderer::Camera::position.y -= 2; Renderer::Camera::refresh_pos(); } };
	EventManager::KeyEvent go_right{ 68, []() {Renderer::Camera::position.x += 2; Renderer::Camera::refresh_pos(); } };

	EventManager::ScrollEvent::get()->callback = [](double amount) {Renderer::Camera::position.z += amount * 10; Renderer::Camera::refresh_pos(); };

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