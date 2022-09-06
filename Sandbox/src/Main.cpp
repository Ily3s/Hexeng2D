#include <thread>
#include <chrono>

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Quad.hpp"
#include "Renderer/Layer.hpp"
#include "Scene.hpp"
#include "Renderer/Camera.hpp"
#include "EventManager/EventManager.hpp"
#include "EventManager/InputEvent.hpp"
#include "EventManager/Button.hpp"
#include "Physics/Physics.hpp"
#include "Renderer/DefaultShaders.glsl"
#include "Renderer/Text.hpp"
#include "Color.hpp"
#include "Renderer/Animation.hpp"

#include "Player.hpp"
#include "Shaders.glsl"

int main()
{

	using namespace Hexeng;

	Settings::window_name = "Sandbox";

	Renderer::init();
	
	Renderer::Shader custom_shader{ Hexeng::Renderer::basic_vs, custom_fs };
	custom_shader.add_necessary_uniforms();

	Color3 frame_color{ 1.0f, 1.0f, 1.0f };
	std::vector<Renderer::Shader*> sh{ &custom_shader };
	Renderer::Uniform<Color3> u_color{ {
		{Renderer::UniformArgType::NAME, "u_color"},
		{Renderer::UniformArgType::CONTROLLLER, &frame_color},
		{Renderer::UniformArgType::SHADERS, &sh} } };
	u_color.add_shaders({ &custom_shader });

	Renderer::Texture frame_tex{ "res/frame.png", {{Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
	Renderer::Square frame{ {0, 1000}, 15.0f, &frame_tex, true, &custom_shader };
	Physics::HitBox frame_hb{ {{Vec2<int>{0, 1000} - Vec2<int>{frame_tex.get_size() * 15} / 2, Vec2<int>{0, 1000} + Vec2<int>{frame_tex.get_size() * 15} / 2}}, 0, false };

	Renderer::Texture example{ "res/example.png", {{Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
	Renderer::Square square{ { 0, 0 }, 5.0f, &example };
	Renderer::Square square2{ { -100, -100 }, 35.0f, &example };
	Renderer::Square square3{ { 2000, -250 }, 500, &example, false };

	Renderer::Animation anim_sq{ { 
		{[&square](float t) {square.rotation += t * 9; }, [&square, &frame_tex]() {square.access_texture() = &frame_tex; }, 100},
		{[&square](float t) {square.rotation -= t * 9; }, [&square, &example]() {square.access_texture() = &example; }, 100} },
		[&square, &example]() {square.access_texture() = &example; square.rotation = 0; }, true };
	anim_sq.play();

	Player player{ {0, 0}, 5.0f, &example };

	EventManager::EventGate in_frame{ [&frame_color, &frame_hb, &player]()
	{
		 if (Physics::From from = Physics::HitBox::where_colliding(player.physics , frame_hb); from != Physics::From::NOWHERE)
		 {
			 if (from == Physics::From::BOT)
				 frame_color = Color3::red;
			 else if (from == Physics::From::TOP)
				 frame_color = Color3::green;
			 else if (from == Physics::From::LEFT)
				 frame_color = Color3::blue;
			 else if (from == Physics::From::RIGHT)
				 frame_color = Color3::yellow;
		 }
	}, Range::LOCAL };

	EventManager::Event not_in_frame{
		[&frame_hb, &player]() {return !Physics::HitBox::is_colliding(player.physics, frame_hb).first; },
		[&frame_color]() {frame_color = Color3::white; }, Range::LOCAL };

	Renderer::Font kunstler{ "res/KUNSTLER.TTF" };
	Renderer::Text txt{ U"Hello, World !", kunstler, {0, 580}, 200, Renderer::HorizontalAlign::CENTER, Renderer::VerticalAlign::TOP, Color3::white };

	Renderer::Texture arrow_unhover{ "res/arrow_unhover.png", {{Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
	Renderer::Texture arrow_hover{ "res/arrow_hover.png", {{Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };

	Vec2<int> top_left = { - 1920 / 2, 1080 / 2 };
	Renderer::Square arrow_up{ top_left + Vec2<int>(115, -50), 50, &arrow_unhover };
	Renderer::Square arrow_down{ top_left + Vec2<int>(115, -190), 50, &arrow_unhover };
	arrow_down.rotation = 180;
	Renderer::Square arrow_left{ top_left + Vec2<int>(45, -120), 50, &arrow_unhover };
	arrow_left.rotation = -90;
	Renderer::Square arrow_right{ top_left + Vec2<int>(185, -120), 50, &arrow_unhover };
	arrow_right.rotation = 90;

	EventManager::Button up_btn{ arrow_up.get_min(), arrow_up.get_max(), []() { return true; }, {
		{ EventManager::ButtonEvent::HOVER, [&arrow_up, &arrow_hover]() { arrow_up.access_texture() = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_up, &arrow_unhover]() { arrow_up.access_texture() = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({0, player.speed}); }}
	} };
	EventManager::Button down_btn{ arrow_down.get_min(), arrow_down.get_max(), []() { return true; }, {
		{ EventManager::ButtonEvent::HOVER, [&arrow_down, &arrow_hover]() { arrow_down.access_texture() = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_down, &arrow_unhover]() { arrow_down.access_texture() = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({0, -player.speed}); }}
	} };
	EventManager::Button left_btn{ arrow_left.get_min(), arrow_left.get_max(), []() { return true; }, {
		{ EventManager::ButtonEvent::HOVER, [&arrow_left, &arrow_hover]() { arrow_left.access_texture() = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_left, &arrow_unhover]() { arrow_left.access_texture() = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({-player.speed, 0}); }}
	} };
	EventManager::Button right_btn{ arrow_right.get_min(), arrow_right.get_max(), []() { return true; }, {
		{ EventManager::ButtonEvent::HOVER, [&arrow_right, &arrow_hover]() { arrow_right.access_texture() = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_right, &arrow_unhover]() { arrow_right.access_texture() = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({player.speed, 0}); }}
	} };

	Renderer::Layer UI_layer{ {&txt, &arrow_up, &arrow_down, &arrow_left, &arrow_right}, 0, Renderer::Position::ABSOLUTE };
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

	EventManager::KeyEvent go_up{ 87, [&player]() {player.physics.move({0, player.speed}); } };			// W
	EventManager::KeyEvent go_left{ 65, [&player]() {player.physics.move({-player.speed, 0}); } };		// A
	EventManager::KeyEvent go_down{ 83, [&player]() {player.physics.move({0, -player.speed}); } };		// S
	EventManager::KeyEvent go_right{ 68, [&player]() {player.physics.move({player.speed, 0}); } };		// D

	EventManager::KeyEvent rotate_left{ 81, [&player]() {player.mesh.rotation -= 1.0f; } };				// Q
	EventManager::KeyEvent rotate_right{ 69, [&player]() {player.mesh.rotation += 1.0f; } };			// E

	EventManager::ScrollEvent::get()->callback = [](double amount) {Renderer::Camera::position.z += amount * 10; Renderer::Camera::refresh_pos(); };

	Physics::HitBox::set_visuallisers_z(750);

	EventManager::KeyPressEvent debug_mode{ 256,[]() { Physics::HitBox::enable_visuallisers = !Physics::HitBox::enable_visuallisers; } }; // ESCAPE

	EventManager::start_looping();

	Hexeng::game_loop();

	EventManager::stop_looping();

	Renderer::stop();

	return 0;
}