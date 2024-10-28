#include <thread>
#include <chrono>
#include <filesystem>

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
#include "Text.hpp"
#include "Color.hpp"
#include "Renderer/Animation.hpp"
#include "Renderer/BatchRenderer.hpp"
#include "Audio/Audio.hpp"
#include "SaveSystem.hpp"

#include "Player.hpp"

int main()
{
	using namespace Hexeng;

	Settings::window_name = "Sandbox";

	Renderer::init();
	Audio::init();

	Renderer::Texture frame_tex{ "res/frame.png", {{Renderer::TexSett::FILTER, GL_NEAREST}} };
	Renderer::Quad frame{ {0, 1000}, 15.0f, &frame_tex };
	Physics::HitBox frame_hb{ {{Vec2<int>{0, 1000} - Vec2<int>{frame_tex.get_size() * 15} / 2, Vec2<int>{0, 1000} + Vec2<int>{frame_tex.get_size() * 15} / 2}}, 0, false };

	Renderer::Texture example{ "res/example.png", {{Renderer::TexSett::FILTER, GL_NEAREST}} };
	Renderer::Quad square{ { 0, 0 }, 5.0f, &example };
	Renderer::Quad square2{ { -100, -100 }, 35.0f, &example };
	Renderer::Square square3{ { 2000, -250 }, 500, &example, false };

	Renderer::Animation anim_sq{ { 
		{[&square](float t) {square.rotation += 1.0f/10 * t; }, [&square, &frame_tex]() {square.texture = &frame_tex; }, 10000},
		{[&square](float t) {square.rotation -= 1.0f/10 * t; }, [&square, &example]() {square.texture = &example; }, 10000} },
		[&square, &example]() {square.texture = &example; square.rotation = 0; }, true };
	anim_sq.play();

	Player player{ {0, 0}, 5.0f, &example };
    
	std::filesystem::create_directory("saves");

	SaveFile save_file;
	int32_t save_var = 0;
	save_file.add_var({ 1, &save_var, 4, 4 });

	Language English{ "res/languages/English.txt" };
	Language French{ "res/languages/French.txt" };

	Language::set_reference_language(&English);
	const Language* language = &English;

	Renderer::Polygon star{
		{ {0, 80}, {20, 20}, {80, 0}, {20, -20}, {0, -80}, {-20, -20}, {-80, 0}, {-20, 20} },
		{0, 1000}
	};

    
	EventManager::EventGate in_frame{ [&frame_hb, &player, &frame, &star,
										&save_var, &save_file,
										&language, &French, &English]()
	{
		 if (Physics::From from = Physics::HitBox::where_colliding(player.physics , frame_hb); from != Physics::From::NOWHERE)
		 {
			 if (from == Physics::From::BOT)
			 {
				 save_file.save("saves/test.save");
				 frame.color_filter = Color4::red;
				 star.color = Color4::red;
			 }
			 else if (from == Physics::From::TOP)
			 {
				 save_file.load("saves/test.save");
				 frame.color_filter = Color4::green;
				 star.color = Color4::green;
			 }
			 else if (from == Physics::From::LEFT)
			 {
				 std::cout << save_var << std::endl;
				 frame.color_filter = Color4::blue;
				 star.color = Color4::blue;
				 language = &French;
				 Text::reload_language();
			 }
			 else if (from == Physics::From::RIGHT)
			 {
				 save_var++;
				 frame.color_filter = Color4::yellow;
				 star.color = Color4::yellow;
				 language = &English;
				 Text::reload_language();
			 }
		 }
	}, Range::LOCAL };


	EventManager::Event not_in_frame{
		[&frame_hb, &player]() {return !Physics::HitBox::is_colliding(player.physics, frame_hb).first; },
		[&star, &frame]() {frame.color_filter = Color4::white; star.color = Color4::white; }, Range::LOCAL };

    
	Font kunstler{ "res/KUNSTLER.TTF" };

	Text txt{ &language, U"Hello, World !", kunstler, {0, 580}, 200, HorizontalAlign::CENTER, VerticalAlign::TOP, Color4::white };

    
	Renderer::Texture arrow_unhover{ "res/arrow_unhover.png", {{Renderer::TexSett::FILTER, GL_NEAREST}} };
	Renderer::Texture arrow_hover{ "res/arrow_hover.png", {{Renderer::TexSett::FILTER, GL_NEAREST}} };

	Vec2<int> top_left = { - 1920 / 2, 1080 / 2 };
	Renderer::Square arrow_up{ top_left + Vec2<int>(115, -50), 50, &arrow_unhover };
	Renderer::Square arrow_down{ top_left + Vec2<int>(115, -190), 50, &arrow_unhover };
	arrow_down.rotation = 180;
	Renderer::Square arrow_left{ top_left + Vec2<int>(45, -120), 50, &arrow_unhover };
	arrow_left.rotation = -90;
	Renderer::Square arrow_right{ top_left + Vec2<int>(185, -120), 50, &arrow_unhover };
	arrow_right.rotation = 90;

	Renderer::TextureAtlas grid_atlas{ "res/GridAtlas.png", {{Renderer::TexSett::FILTER, GL_NEAREST}} };
    Renderer::TALayout layout{Vec2<int>{16, 16}, Vec2<int>{2, 1}};


	Renderer::BatchInstance batch_test{ layout };
    batch_test.texture = &grid_atlas;
	Renderer::BatchQuad batch_quad_1{ &batch_test, Vec2<int>{0, 0}, {-2000, 0}, 30.0f };
	Renderer::BatchQuad batch_quad_2{ &batch_test, Vec2<int>{1, 0}, {2000, 0}, 30.0f };
	batch_test.construct_batch();
	batch_test.position.y = 1000;

	Audio::Music music{ "res/Music.wav" };
	music.play();
	//Audio::SpatialSound sound{ "res/SoundEffect.wav" };
	//sound.play(&batch_quad_1.position);
	//sound.play(&batch_quad_2.position);

	Renderer::Animation animate_batch{ {
		{[&batch_quad_1, &batch_quad_2](float t)
		{batch_quad_1.rotation += 1.0f/100 * t; batch_quad_2.rotation -= 1.0f/100 * t; },
		[]() {}, 100} }, []() {}, true };
	animate_batch.play();

	EventManager::Button up_btn{ arrow_up.get_min(), arrow_up.get_max(), {
		{ EventManager::ButtonEvent::HOVER, [&arrow_up, &arrow_hover]() { arrow_up.texture = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_up, &arrow_unhover]() { arrow_up.texture = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({0, player.speed}); }}
	} };
	EventManager::Button down_btn{ arrow_down.get_min(), arrow_down.get_max(), {
		{ EventManager::ButtonEvent::HOVER, [&arrow_down, &arrow_hover]() { arrow_down.texture = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_down, &arrow_unhover]() { arrow_down.texture = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({0, -player.speed}); }}
	} };
	EventManager::Button left_btn{ arrow_left.get_min(), arrow_left.get_max(), {
		{ EventManager::ButtonEvent::HOVER, [&arrow_left, &arrow_hover]() { arrow_left.texture = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_left, &arrow_unhover]() { arrow_left.texture = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({-player.speed, 0}); }}
	} };
	EventManager::Button right_btn{ arrow_right.get_min(), arrow_right.get_max(), {
		{ EventManager::ButtonEvent::HOVER, [&arrow_right, &arrow_hover]() { arrow_right.texture = &arrow_hover; } },
		{ EventManager::ButtonEvent::UNHOVER, [&arrow_right, &arrow_unhover]() { arrow_right.texture = &arrow_unhover; } },
		{ EventManager::ButtonEvent::KEEP_CLICKING, [&player]() { player.physics.move({player.speed, 0}); }}
	} };

	Renderer::Layer UI_layer{ { &txt, &arrow_up, &arrow_down, &arrow_left, &arrow_right}, 0, Renderer::Position::ABSOLUTE };
	Renderer::Layer fore_ground{ {&player.mesh, &square3, &frame, &batch_test, &star}, 750 };
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

	EventManager::KeyEvent rotate_left{ 81, [&player]() {player.mesh.rotation -= 1.0f; } };			// Q
	EventManager::KeyEvent rotate_right{ 69, [&player]() {player.mesh.rotation += 1.0f; } };		// E

	EventManager::ScrollEvent::get()->callback = [](double amount) {Renderer::Camera::position.z += amount * 10; Renderer::Camera::refresh_pos(); };

	Physics::HitBox::set_visuallisers_z(750);

	EventManager::KeyPressEvent debug_mode{ 256,[]() { Physics::HitBox::enable_visuallisers = !Physics::HitBox::enable_visuallisers; } }; // ESCAPE

	EventManager::start_looping();

	Hexeng::game_loop();

	Audio::terminate();

	EventManager::stop_looping();

	Renderer::stop();

	return 0;
}
