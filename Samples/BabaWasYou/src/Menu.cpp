#include "Menu.hpp"
#include "Player.hpp"

#include "EventManager/Button.hpp"
#include "Renderer/Animation.hpp"
#include "SaveSystem.hpp"
#include "Renderer/Camera.hpp"

#include <filesystem>
#include <fstream>
#include <cstring>

Hexeng::Renderer::Quad logo;
Hexeng::EventManager::Button play_btn;

uint8_t* logo_pixel_buffer;

Hexeng::Audio::Sound hover_sound;
Hexeng::Audio::Sound click_sound;

SelectionItem* current_selection_item = nullptr;

Hexeng::Renderer::Animation click_play{ {
	{[](float) {}, []() {click_sound.play(); }, 0},
		Hexeng::Renderer::AnimationStep{
			[](float t) {logo.scale += t / 50; logo.opacity -= t / 100; },
			[]() { Hexeng::Scene::switch_scene(1); logo.scale = 1; logo.opacity = 1; }, 100}
	},
[]() {} };

Hexeng::Renderer::Animation hover_play{ {
	{[](float) {}, []() {hover_sound.play(); }, 0},
	Hexeng::Renderer::AnimationStep{
			[](float t) {logo.scale += t / 400; },
			[]() { /*todo : sound*/ }, 40}
	},
[]() {} };

Hexeng::Renderer::Animation unhover_play{ {
		Hexeng::Renderer::AnimationStep{
			[](float t) {logo.scale -= t / 400; },
			[]() { logo.scale = 1;/*todo : sound*/ }, 50}
	},
[]() {} };

Hexeng::Renderer::Animation cool_anim{ {
		Hexeng::Renderer::AnimationStep{
			[](float t) { logo.position = -Hexeng::EventManager::mouse_position/8.0f; },
			[]() { /*todo : sound*/ }, 9999}
	},
[]() {}, true };

int was_hovered = false;

Hexeng::Renderer::ToBeInit init_menu{ []()
	{
		hover_sound = {"res/hover.wav"};
		click_sound = { "res/pop.wav" };
		logo_pixel_buffer = Hexeng::Renderer::Texture::to_pixel_buff("res/gem.png");
		logo = Hexeng::Renderer::Quad{{0, 0}, {300, 300}, &gem_texture};
		play_btn = Hexeng::EventManager::Button{ {-1000, -1000}, {1000, 1000}, {}, 0, Hexeng::Range::LOCAL };
		play_btn.click = []() { click_play.play(); };
		play_btn.hover = []() { hover_play.play(); };
		play_btn.unhover = []() {unhover_play.play(); };
		play_btn.condition = []() -> bool
		{
			Hexeng::Vec2<double> delta = Hexeng::EventManager::mouse_position - Hexeng::Vec2<double>(logo.position.x, logo.position.y);
			Hexeng::Vec2<double> pos_to_logo = (delta / (Hexeng::Vec2<double>{300.0, 300.0} * (double)logo.scale)) + 0.5;
			if (pos_to_logo.x > 1 || pos_to_logo.x < 0 || pos_to_logo.y > 1 || pos_to_logo.y < 0)
			{
				if (was_hovered)
					play_btn.unhover();
				was_hovered = false;
				return false;
			}
			Hexeng::Vec2<int> coords = Hexeng::Vec2<int>(pos_to_logo * 16.0);
			if (logo_pixel_buffer[(coords.y * 16 + coords.x) * 4 + 3])
			{
				if (!was_hovered)
					play_btn.hover();
				was_hovered = true;
				return true;
			}
			else
			{
				if (was_hovered)
					play_btn.unhover();
				was_hovered = false;
				return false;

			}
		};
		cool_anim.play();
	}, 4
};

Hexeng::Renderer::ToBeDelete delete_pixel_buff{ logo_pixel_buffer, []() {Hexeng::Renderer::Texture::free_pixel_buff(logo_pixel_buffer); } };

Hexeng::Renderer::Layer menu_layer{ {&logo}, 1, Hexeng::Renderer::Position::ABSOLUTE };

Hexeng::Renderer::Quad scroll_bar;
Hexeng::Renderer::Quad scroll_bar_core;

Hexeng::Scene menu_scene{ 0, {
	{Hexeng::SceneComponent::LAYERS, {&menu_layer}},
	{Hexeng::SceneComponent::EVENTS, {&play_btn}}
} };

Hexeng::Renderer::Layer map_selection_layer{ {}, 469 };

Hexeng::Renderer::Layer scroll_bar_layer{ {&scroll_bar_core, &scroll_bar}, 1, Hexeng::Renderer::Position::ABSOLUTE };

Hexeng::Scene map_selection{ 1, {
	{Hexeng::SceneComponent::LAYERS, {&map_selection_layer, &scroll_bar_layer}}
} };

SelectionItem::~SelectionItem()
{
	Hexeng::Renderer::Texture::free_pixel_buff(map);
}

std::vector<uint8_t> scores_maps;
std::vector<uint64_t> actual_scores;
Hexeng::SaveFile scores_save;
Hexeng::SaveVar scores_maps_var{ 0, nullptr, 1,
				[](void** ptr) { *ptr = scores_maps.size()? & scores_maps[0]:nullptr; return scores_maps.size(); },
				[](size_t size, void** ptr) {scores_maps.resize(size); *ptr = &scores_maps[0]; }
};
Hexeng::SaveVar actual_scores_var{ 1, nullptr, 8,
				[](void** ptr) {*ptr = actual_scores.size()? & actual_scores[0]:nullptr; return actual_scores.size() * 8; },
				[](size_t size, void** ptr) {actual_scores.resize(size/8); *ptr = &actual_scores[0]; }
};

std::vector<SelectionItem> map_items;

Hexeng::Renderer::Texture pass_tex;
Hexeng::Renderer::Texture no_pass_tex;

Hexeng::Renderer::ToBeInit init_game_selection{ []()
	{
		pass_tex = Hexeng::Renderer::Texture{"res/pass.png", {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}}};
		no_pass_tex = Hexeng::Renderer::Texture{ "res/no_pass.png", {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };

		std::filesystem::path maps_folder{"maps"};
		int map_nb = 0;
		for (const auto& map_file : std::filesystem::directory_iterator{ maps_folder })
			map_nb++;
		map_items.reserve(map_nb);
		int i = 0;

		scores_save.add_var(scores_maps_var);
		scores_save.add_var(actual_scores_var);

		try
		{
			scores_save.load("scores.hxg_save");
		} catch (const std::runtime_error& e){}

		for (const auto& map_file : std::filesystem::directory_iterator{ maps_folder })
		{
			map_items.emplace_back();
			map_items.back().map = Hexeng::Renderer::Texture::to_pixel_buff(map_file.path().string());
			map_items.back().icon_texture = Hexeng::Renderer::Texture{map_file.path().string(),
				{{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
			map_items.back().icon = Hexeng::Renderer::Quad{ {-250, 450 - i * 160}, {150, 150}, &map_items.back().icon_texture };
			map_items.back().button_mesh = Hexeng::Renderer::Quad{ {0, 450 - 160 * i}, {650, 150}, nullptr };
			map_items.back().pass_mesh = Hexeng::Renderer::Quad{ {-100, 410 - 160 * i}, {64, 64}, &no_pass_tex };
			map_items.back().button_mesh.color = Hexeng::Color4::white;
			map_items.back().button_mesh.opacity = 0.15f;
			map_items.back().button = Hexeng::EventManager::Button{
				map_items.back().button_mesh.position - map_items.back().button_mesh.get_size() / 2,
				map_items.back().button_mesh.position + map_items.back().button_mesh.get_size() / 2,
				{}, 0, Hexeng::Range::LOCAL
			};
			map_items.back().button.condition = [i]() {
				map_items[i].button.min = map_items[i].button_mesh.position - map_items[i].button_mesh.get_size() / 2 - *Hexeng::Renderer::Camera::xy_position;
				map_items[i].button.max = map_items[i].button_mesh.position + map_items[i].button_mesh.get_size() / 2 - *Hexeng::Renderer::Camera::xy_position;
				return true;
			};
			map_items.back().button.hover = [i]() {
				hover_sound.play();
				map_items[i].button_mesh.opacity = 0.3f;
			};
			map_items.back().button.unhover = [i]() {
				map_items[i].button_mesh.opacity = 0.15f;
			};
			map_items.back().button.click = [i]() { Hexeng::Renderer::pending_actions.push_back([i]() {
				current_selection_item = &map_items[i];
				std::memcpy(map, map_items[i].map, 1024);
				build_board(map_items[i].map);
				Hexeng::Scene::switch_scene(2);
				start = std::chrono::high_resolution_clock::now();
			}); };
			std::u32string map_path = map_file.path().u32string();
			std::u32string map_name = {
				std::find_if(map_path.begin(), map_path.end(), [](char32_t c) {return c == '/' || c == '\\'; }) + 1,
				std::find(map_path.begin(), map_path.end(), '.') };
			map_items.back().map_name = Hexeng::Text{ map_name, roboto, {75, 475 - i * 160}, 50};
			map_selection_layer.meshes.push_back(&map_items.back().button_mesh);
			map_selection_layer.meshes.push_back(&map_items.back().icon);
			map_selection_layer.meshes.push_back(&map_items.back().map_name);
			map_selection_layer.meshes.push_back(&map_items.back().pass_mesh);

			// TODO : SCORE TEXT

			map_items.back().score_text = Hexeng::Text{ U"-------", roboto, {75, 400 - i * 160}, 50 };

			map_selection_layer.meshes.push_back(&map_items.back().score_text);

			for (int j = 0; j < scores_maps.size(); j += 1024)
			{
				if (std::memcmp(&scores_maps[j], map_items.back().map, 1024) == 0)
				{
					map_items.back().index_in_save = j / 1024;
					uint64_t score = actual_scores[j/1024];
					map_items.back().score_text = Hexeng::Text{ time_to_string(score), roboto, {75, 400 - i * 160}, 50};
					map_items.back().pass_mesh.access_texture() = &pass_tex;
				}
			}

			map_selection.events.push_back(&map_items.back().button);
			i++;
		}
		Hexeng::EventManager::ScrollEvent::get()->callback = scroll;
	}, 5
};

Hexeng::EventManager::EventGate scroll_correction{ []() {
	if (Hexeng::scene_id == 1)
	{
		int max_y_pos = 375 + 525 - 160 * (map_items.size()-1);
		max_y_pos = max_y_pos > 0 ? 0 : max_y_pos;

		if (Hexeng::Renderer::Camera::position.y < max_y_pos)
		{
			Hexeng::Renderer::Camera::position.y += std::ceil((float)(max_y_pos - Hexeng::Renderer::Camera::position.y) * 0.1f);
			Hexeng::Renderer::Camera::refresh_pos();
		}

		if (Hexeng::Renderer::Camera::position.y > 0)
		{
			Hexeng::Renderer::Camera::position.y -= std::floor((float)Hexeng::Renderer::Camera::position.y * 0.1f);
			Hexeng::Renderer::Camera::refresh_pos();
		}
	}
} };

void scroll(double amount)
{
	if (Hexeng::scene_id == 1)
	{
		Hexeng::Renderer::Camera::position.y += amount * 50;
		Hexeng::Renderer::Camera::refresh_pos();
	}
}

Hexeng::EventManager::Button scroll_bar_btn_click{ {0, 0}, {0, 0}, {} };
Hexeng::EventManager::Button scroll_bar_btn_unclick{ {-960, -540}, {960, 540}, {} };

int mouse_pos_y_start = 0;
int camera_pos_y_start = 0;

bool scroll_bar_track = false;

Hexeng::EventManager::Event scroll_bar_track_evt{ []() {return Hexeng::scene_id == 1 && scroll_bar_track; }, []() {
	int total_height = 160 * map_items.size();
	total_height = total_height < 1080 ? 1080 : total_height;
	double diff_mouse_y = Hexeng::EventManager::mouse_position.y - mouse_pos_y_start;
	int new_cam_y = camera_pos_y_start + diff_mouse_y / 1080 * total_height;
	int max_y_pos = 375 + 525 - 160 * (map_items.size() - 1);
	if (new_cam_y < max_y_pos || new_cam_y > 0)
		return;
	Hexeng::Renderer::Camera::position.y = new_cam_y;
	Hexeng::Renderer::Camera::refresh_pos();
} };

Hexeng::Renderer::ToBeInit init_scroll_bar{ []() {
	scroll_bar_core = Hexeng::Renderer::Quad{{800, 0}, {20, 1080}, nullptr};
	scroll_bar_core.color = Hexeng::Color4::white;
	scroll_bar_core.opacity = 0.1f;
	int total_height = 160 * map_items.size();
	total_height = total_height < 1080 ? 1080 : total_height;
	scroll_bar = Hexeng::Renderer::Quad{ {800, 540 - 540*1080/total_height}, {20, 1080 * 1080 / total_height}, nullptr};
	scroll_bar.color = Hexeng::Color4::white;
	scroll_bar.opacity = 0.2f;
	scroll_bar_btn_click.click = []() {
		scroll_bar.opacity = 0.4f;
		mouse_pos_y_start = Hexeng::EventManager::mouse_position.y;
		camera_pos_y_start = Hexeng::Renderer::Camera::position.y;
		scroll_bar_track = true;
	};
	scroll_bar_btn_unclick.unclick = []() {
		scroll_bar.opacity = 0.2f;
		scroll_bar_track = false;
	};
}, 6 };

Hexeng::EventManager::EventGate update_scroll_bar{ []() {
	if (Hexeng::scene_id == 1)
	{
		double total_height = 160 * map_items.size();
		total_height = total_height < 1080 ? 1080 : total_height;
		scroll_bar.position.y = 540 + Hexeng::Renderer::Camera::position.y/total_height * 1080 - 540 * 1080 / total_height;
		scroll_bar_btn_click.min = scroll_bar.position - Hexeng::Vec2<int>{20, (int)(1080 * 1080 / total_height)} / 2;
		scroll_bar_btn_click.max = scroll_bar.position + Hexeng::Vec2<int>{20, (int)(1080 * 1080 / total_height)} / 2;
	}
} };
