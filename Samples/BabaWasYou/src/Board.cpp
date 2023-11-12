#include "Renderer/Camera.hpp"
#include "Scene.hpp"
#include "Renderer/Animation.hpp"

#include "Board.hpp"
#include "Player.hpp"
#include "Menu.hpp"

#include <string>
#include <sstream>

Hexeng::Scene board_scene{ 2, {{Hexeng::SceneComponent::LAYERS, {&board_layer, &player_layer, &UI_layer}}} };

using Hexeng::Vec2;

std::array<Tile, 256> board;
std::array<Behaviour, 256> behaviours;
std::array<Hexeng::Renderer::Texture, 256> textures;
std::array<Hexeng::Renderer::Texture, 256> b_textures;

Hexeng::Renderer::Layer board_layer{{}, 1, Hexeng::Renderer::Position::ABSOLUTE};

Hexeng::Renderer::ToBeInit init_textures{ []()
	{
		for (int i = 0; i < 10; i++)
		{
			std::stringstream filepath;
			filepath << "res/tiles/" << i << ".png";
			textures[i] = Hexeng::Renderer::Texture{ filepath.str(), {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
			std::stringstream b_filepath;
			b_filepath << "res/tiles/" << i << "b.png";
			std::ifstream file{ b_filepath.str() };
			if (file)
			{
				file.close();
				b_textures[i] = Hexeng::Renderer::Texture{ b_filepath.str(), {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
			}
		}
	}
};

float board_scale = 1.0f;

void build_board(uint8_t* pixel_buffer)
{
	float outermost = 0;
	for (int i = 0; i < 256; i++)
	{
		board[i] = Tile(i % 16, i / 16, pixel_buffer + i*4);
		board_layer.meshes.push_back(&board[i].mesh);

		if (*(pixel_buffer + i * 4) == 1)
			Player::init_player(i);
		if (*(pixel_buffer + i * 4) != 4)
			outermost = std::max(outermost, std::max(std::abs((float)(i / 16) - 7.5f), std::abs((float)(i % 16) - 7.5f)));
	}
	board_scale = 6.8f / outermost;
	board_layer.scale = board_scale;
	player_layer.scale = board_scale;
}

Tile::Tile(int x, int y, uint8_t* data)
	: mesh(pos2coord(x, y), { 64, 64 }, &textures[data[0]])
{
	id = data[0];
	var1 = data[1];
	var2 = data[2];
	var3 = data[3];

	physics = behaviours[id].physics;

	if (id == 1 && !gems_count)
	{
		gems_count = var1;
		update_gems_display();
	}
}

Hexeng::Audio::Sound door_close;
Hexeng::Audio::Sound door_open;

Hexeng::Renderer::ToBeInit init_door_sounds{ []() {
	door_open = {"res/door_open.wav"};
	door_close = { "res/door_close.wav" };
} };

void send_pulse(Tile* tile)
{
	tile->pulse++;
	if (tile->pulse == 1 || behaviours[tile->id].multi_pulse)
		behaviours[tile->id].activate(tile);
}

void stop_pulse(Tile* tile)
{
	tile->pulse--;
	if (tile->pulse == 0 || behaviours[tile->id].multi_pulse)
		behaviours[tile->id].deactivate(tile);
}

Hexeng::Renderer::ToBeInit init_behaviours{ []()
	{
		behaviours[3].physics = true;
		behaviours[2].physics = true;
		behaviours[2].press = [](Tile* tile, Player* player) {
			tile->id = 0;
			tile->physics = false;
			tile->mesh.access_texture() = &textures[0];
			gems_count++;
			Hexeng::Renderer::pending_actions.push_back([]() {update_gems_display(); });
			map[(tile - &board[0]) * 4] = 0;
			loot_sound.play();
		};
		behaviours[4].physics = true;
		behaviours[5].physics = true;
		behaviours[5].press = [](Tile* tile, Player* player) {
			win();
		};
		behaviours[6].walks = [](Tile* tile, Player* player) {
			win();
		};
		behaviours[7].physics = true;
		behaviours[7].activate = [](Tile* tile) {
			if (tile->pulse < tile->var1)
				return;

			door_open.play();
			tile->mesh.access_texture() = &b_textures[7];
			tile->physics = false;
		};
		behaviours[7].deactivate = [](Tile* tile) {
			if (tile->pulse != tile->var1 - 1)
				return;

			door_close.play();
			tile->mesh.access_texture() = &textures[7];
			tile->physics = true;
		};
		behaviours[7].multi_pulse = true;
		behaviours[8].walks = [](Tile* tile, Player* player) {
			tile->mesh.access_texture() = &b_textures[8];
			send_pulse(&board[tile->var1]);
		};
		behaviours[8].leave = [](Tile* tile, Player* player) {
			tile->mesh.access_texture() = &textures[8];
			stop_pulse(&board[tile->var1]);
		};
		behaviours[9].walks = [](Tile* tile, Player* player) {
			tile->mesh.access_texture() = &b_textures[8];
			send_pulse(&board[tile->var1]);
			send_pulse(&board[tile->var2]);
		};
		behaviours[9].leave = [](Tile* tile, Player* player) {
			tile->mesh.access_texture() = &textures[8];
			stop_pulse(&board[tile->var1]);
			stop_pulse(&board[tile->var2]);
		};
	}
};

uint64_t timer = 0;

Hexeng::Text timer_text;

std::chrono::steady_clock::time_point start;

std::u32string time_to_string(uint64_t time)
{
	std::u32string output;
	output.push_back('0' + ((time / 3600000) % 10));
	output.push_back('0' + ((time / 360000) % 10));
	output.push_back(':');
	output.push_back('0' + ((time / 60000) % 6));
	output.push_back('0' + ((time / 6000) % 10));
	output.push_back(':');
	output.push_back('0' + ((time / 1000) % 6));
	output.push_back('0' + ((time / 100) % 10));
	output.push_back(':');
	output.push_back('0' + ((time / 10) % 10));
	output.push_back('0' + (time % 10));
	return output;
}

Hexeng::Renderer::ToBeInit init_timer_text
{[](){
	timer_text = Hexeng::Text{ time_to_string(timer), roboto, {-900, 200}, 60, Hexeng::HorizontalAlign::LEFT };
	UI_layer.meshes.push_back(&timer_text);
	start = std::chrono::high_resolution_clock::now();
}, 3};

Hexeng::EventManager::EventGate update_timer{ []()
	{
		timer = (std::chrono::high_resolution_clock::now() - start).count() / 10000000;
		Hexeng::Renderer::pending_actions.push_back([]() {
			timer_text = Hexeng::Text{ time_to_string(timer), roboto, {-900, 200}, 60, Hexeng::HorizontalAlign::LEFT };
		});
	}
};

Hexeng::Audio::Sound win_sound;
Hexeng::Renderer::ToBeInit init_win_sound{ []() {win_sound = Hexeng::Audio::Sound{"res/win.wav"}; }, 1 };

Hexeng::Renderer::Quad win_gem;
Hexeng::Renderer::Texture win_gem_texture;
Hexeng::Renderer::Layer win_layer{ {&win_gem}, 1, Hexeng::Renderer::Position::ABSOLUTE, Hexeng::Range::GLOBAL };

Hexeng::Renderer::ToBeInit init_win_gem{ []() {
	win_gem_texture = Hexeng::Renderer::Texture{"res/win_gem.png", {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}}};
	win_gem = Hexeng::Renderer::Quad{ {0, -900}, 16, &win_gem_texture };
}, 1 };

Hexeng::Renderer::Animation win_animation{ {
		Hexeng::Renderer::AnimationStep{
		[](float t) {win_gem.position.y += t;
					board_layer.opacity -= t / 900;
					player_layer.opacity -= t / 900;
					UI_layer.opacity -= t / 900; },
		[]() {map_selection_layer.opacity = 0;
					scroll_bar_layer.opacity = 0;
					}, 900 },
		Hexeng::Renderer::AnimationStep{[](float t){}, [](){
					Hexeng::Scene::switch_scene(1);
					gems_count = 0;
					Hexeng::Renderer::pending_actions.push_back([]() {
						board_layer.opacity = 1;
						player_layer.opacity = 1;
						UI_layer.opacity = 1;
						Player::storage.clear(); });
					}, 200},
		Hexeng::Renderer::AnimationStep{
		[](float t) {win_gem.position.y -= t;
					map_selection_layer.opacity += t / 900;
					scroll_bar_layer.opacity += t / 900; },
		[]() {win_gem.position.y = -900; }, 900 }
},[]() {} };

extern Hexeng::Renderer::Texture pass_tex;

void win()
{
	win_sound.play();
	if (current_selection_item->index_in_save == -1)
	{
		current_selection_item->index_in_save = actual_scores.size();
		actual_scores.push_back(timer);
		scores_maps.resize(scores_maps.size() + 1024);
		std::memcpy(&scores_maps[scores_maps.size() - 1024], current_selection_item->map, 1024);
	}
	if (timer < actual_scores[current_selection_item->index_in_save])
	{
		actual_scores[current_selection_item->index_in_save] = timer;
	}
	Hexeng::Renderer::pending_actions.push_back([]() {
		current_selection_item->score_text = Hexeng::Text{
			time_to_string(actual_scores[current_selection_item->index_in_save]), roboto,
			current_selection_item->score_text.position, 50};
		current_selection_item->pass_mesh.access_texture() = &pass_tex;
		win_animation.play();
	});
}