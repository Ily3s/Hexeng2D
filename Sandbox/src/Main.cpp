#include <thread>
#include <chrono>

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Presets/BasicQuad.hpp"
#include "Renderer/Layer.hpp"
#include "Scene.hpp"
#include "Renderer/Camera.hpp"
#include "EventManager/EventManager.hpp"
#include "EventManager/InputEvent.hpp"
#include "Physics/Physics.hpp"
#include "Renderer/Presets/Basic.shader"
#include "Renderer/Text.hpp"
#include "Color.hpp"

#include "Player.hpp"
#include "Shaders.glsl"

int main()
{

	using namespace Hexeng;

	Settings::window_name = "Sandbox";

	Renderer::init();

	Renderer::Font kunstler{ "res/KUNSTLER.TTF" };

	Renderer::Text txt{ U"Hello, World !", kunstler, {0, 580}, 200, Renderer::HorizontalAlign::CENTER, Renderer::VerticalAlign::TOP, Color3::white };

	Renderer::Shader custom_shader{ Hexeng::Renderer::Presets::basic_vs, custom_fs };
	custom_shader.add_necessary_uniforms();

	Color3 color{ 0.0f, 0.0f, 1.0f };
	Renderer::Uniform<Color3> u_color = { "u_color", &color, {&custom_shader} };

	Renderer::Texture frame_tex{ "res/frame.png", GL_NEAREST };
	Renderer::Presets::BasicSquare frame{ {0, 1000}, 15.0f, &frame_tex, true, &custom_shader };
	Physics::HitBox frame_hb{ {{Vec2<int>{0, 1000} - Vec2<int>{frame_tex.get_size() * 15} / 2, Vec2<int>{0, 1000} + Vec2<int>{frame_tex.get_size() * 15} / 2}}, 0, false };

	Renderer::Texture example{ "res/example.png", GL_NEAREST };
	Renderer::Presets::BasicSquare square{ { 0, 0 }, 5.0f, &example };
	Renderer::Presets::BasicSquare square2{ { -100, -100 }, 35.0f, &example };
	Renderer::Presets::BasicSquare square3{ { 2000, -250 }, 500, &example, false };

	Player player{ {0, 0}, 5.0f, &example };

	EventManager::EventGate in_frame{ [&color, &frame_hb, &player]()
	{
		 if (Physics::From from = Physics::HitBox::where_colliding(player.physics , frame_hb); from != Physics::From::NOWHERE)
		 {
			 if (from == Physics::From::BOT)
				 color = Color3::red;
			 else if (from == Physics::From::TOP)
				 color = Color3::green;
			 else if (from == Physics::From::LEFT)
				 color = Color3::blue;
			 else if (from == Physics::From::RIGHT)
				 color = Color3::yellow;
		 }
	}, Range::LOCAL };

	EventManager::Event not_in_frame{
		[&frame_hb, &player]() {return !Physics::HitBox::is_colliding(player.physics, frame_hb).first; },
		[&color]() {color = Color3::white; }, Range::LOCAL };

	Renderer::Layer UI_layer{ txt.chars, 0, Renderer::Position::ABSOLUTE };
	Renderer::Layer fore_ground{ {&player.mesh, &square3, &frame}, 750 };
	Renderer::Layer back_ground1{ {&square}, 500 };
	Renderer::Layer back_ground2{ {&square2}, 800 };

	Physics::HitBox hb = Physics::HitBox({ {{2000, -250}, {2500, 250}} }, 10);

	Scene first_scene{ 1, {
		{ SceneComponent::LAYERS, { &fore_ground , &back_ground1, &back_ground2, &UI_layer } },
		{ SceneComponent::EVENTS, { &in_frame, &not_in_frame } },
		{ SceneComponent::HITBOXES, { &hb, &frame_hb } },
		{ SceneComponent::PHYS_ENTITIES, { &player.physics } }
	} };

	scene_id = 1;

	EventManager::KeyEvent go_up{ 87, [&player]() {player.physics.move({0, 4}); } };		// W
	EventManager::KeyEvent go_left{ 65, [&player]() {player.physics.move({-4, 0}); } };		// A
	EventManager::KeyEvent go_down{ 83, [&player]() {player.physics.move({0, -4}); } };		// S
	EventManager::KeyEvent go_right{ 68, [&player]() {player.physics.move({4, 0}); } };		// D

	EventManager::KeyEvent rotate_left{ 81, [&player]() {player.mesh.rotation -= 1.0f; } };	// Q
	EventManager::KeyEvent rotate_right{ 69, [&player]() {player.mesh.rotation += 1.0f; } };// E

	EventManager::ScrollEvent::get()->callback = [](double amount) {Renderer::Camera::position.z += amount * 10; Renderer::Camera::refresh_pos(); };

	Physics::HitBox::set_visuallisers_z(750);

	EventManager::KeyPressEvent debug_mode{ 256,[]() { Physics::HitBox::enable_visuallisers = !Physics::HitBox::enable_visuallisers; } }; // ESCAPE

	EventManager::start_looping();

	Hexeng::game_loop();

	EventManager::stop_looping();

	Renderer::stop();

	return 0;
}