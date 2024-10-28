#include "Menu.hpp"
#include "Utils.hpp"
#include "GameSelection.hpp"
#include "Settings.hpp"

Hexeng::Renderer::Quad background;
Hexeng::Renderer::Quad logo_quad;
Hexeng::Text menu_title;
DefaultButton new_game_btn;
DefaultButton load_game_btn;
DefaultButton settings_btn;
DefaultButton exit_btn;

Hexeng::Renderer::Layer menu_layer{ {&background, &logo_quad, &menu_title, &new_game_btn.mesh, &load_game_btn.mesh, &settings_btn.mesh, &exit_btn.mesh},
									0, Hexeng::Renderer::Position::ABSOLUTE};

Hexeng::Scene menu_scene{ 0, {{Hexeng::SceneComponent::LAYERS, {&menu_layer}},
							{Hexeng::SceneComponent::EVENTS, {&new_game_btn.button, &load_game_btn.button, &settings_btn.button, &exit_btn.button}}} };

Hexeng::Renderer::ToBeInit init_menu
{ []()
	{
		background = Hexeng::Renderer::Quad{{0, 0}, {99999, 99999}, nullptr};
		background.color = { "#051423ff" };

		logo_quad = Hexeng::Renderer::Quad{ {0, 170}, 0.8f, &full_logo };

		menu_title = Hexeng::Text{ U"DUDUDUDUDUEL !", roboto_font, {0 , 430}, 108 };

		new_game_btn = DefaultButton{ U"Start New Game", []() {
			player1_score = 0;
			player2_score = 0;
			update_scores();
			Hexeng::Scene::switch_scene(2);
		}, {0, -150}, 10.0f, background.color };
		load_game_btn = DefaultButton{ U"Load Game", []() {
			try { score_save.load("saves/scores.hxg_save"); }
			catch (const std::exception& e) {}
			update_scores();
			Hexeng::Scene::switch_scene(2);
		}, {0, -250}, 10.0f, background.color };
		settings_btn = DefaultButton{ U"Settings", []() { Hexeng::Scene::switch_scene(1); }, {0, -350}, 10.0f, background.color };
		exit_btn = DefaultButton{ U"Exit", []() { settings_save.save("saves/settings.hxg_save"); Hexeng::exit = true; }, {0, -450}, 10.0f, background.color };

	}, 2
};