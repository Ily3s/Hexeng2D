#include "GameSelection.hpp"
#include "Menu.hpp"
#include "Utils.hpp"
#include "Settings.hpp"

int player1_score = 0, player2_score = 0;

Hexeng::Text title;
Hexeng::Text p1_score_txt;
Hexeng::Text p2_score_txt;
Hexeng::Renderer::Quad frame_outline;
Hexeng::Renderer::Quad frame;

GameIcon a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t;

Hexeng::Renderer::Layer game_selection_layer;

Hexeng::Renderer::Quad escape_fog;
Hexeng::Renderer::Quad escape_background;
Hexeng::Renderer::Quad escape_frame;
static Hexeng::EventManager::KeyPressEvent press_escape;

static DefaultButton menu_btn;
static DefaultButton save_btn;
static DefaultButton exit_btn;
static DefaultButton settings_btn;

Hexeng::Renderer::Layer escape_layer{ {&escape_fog, &escape_background, &escape_frame,
	&menu_btn.mesh, &save_btn.mesh, &exit_btn.mesh, &settings_btn.mesh}, 0, Hexeng::Renderer::Position::ABSOLUTE };

Hexeng::Scene game_selection_scene{ 2, {
	{Hexeng::SceneComponent::LAYERS, {&game_selection_layer, &escape_layer}},
	{Hexeng::SceneComponent::EVENTS, {&menu_btn.button, &save_btn.button, &exit_btn.button, &settings_btn.button}}
}};

DefaultButton game_selection_btn;
DefaultButton p1_surrender;
DefaultButton p2_surrender;
DefaultButton ig_settings_btn;

Hexeng::Text p1_wins;
Hexeng::Text p2_wins;
Hexeng::Text draw_text;
DefaultButton game_selection_win;
Hexeng::Renderer::Quad ig_win_line;

Hexeng::Renderer::Layer ig_win_layer{ {&escape_fog, &escape_background, &escape_frame, &p1_wins, &p2_wins, &draw_text, &game_selection_win.mesh, &ig_win_line},
0, Hexeng::Renderer::Position::ABSOLUTE };

Hexeng::Renderer::Layer ig_escape_layer{{&escape_fog, &escape_background, &escape_frame,
	&game_selection_btn.mesh, &p1_surrender.mesh, &p2_surrender.mesh, &ig_settings_btn.mesh}, 0, Hexeng::Renderer::Position::ABSOLUTE};
std::vector<Hexeng::EventManager::Event*> ig_base_evts{&game_selection_btn.button, &p1_surrender.button, &p2_surrender.button, &game_selection_win.button, &ig_settings_btn.button};

Hexeng::SaveVar p1_save{ 1, &player1_score, 4, 4 };
Hexeng::SaveVar p2_save{ 2, &player2_score, 4, 4 };

Hexeng::SaveFile score_save;

namespace Tron
{
	void ungenerate_apple();
}

namespace HiddenConnect4
{
	void on_game_begin();
}

Hexeng::Renderer::ToBeInit init_game_selection
{
	[]()
	{

		score_save.add_var(p1_save);
		score_save.add_var(p2_save);

		game_selection_layer = { {&background, &frame_outline, &frame, &title, &p1_score_txt, &p2_score_txt},
			0, Hexeng::Renderer::Position::ABSOLUTE };

		title = Hexeng::Text{ &language_switch, U"Duel Selection", roboto_font, {0, 375}, 125 };

		p1_score_txt = Hexeng::Text{U"P1 0", roboto_font, {-700, 350}, 60};;
		p1_score_txt.color_filter = Hexeng::Color4::cyan;
		p2_score_txt = Hexeng::Text{ U"P2 0", roboto_font, {700, 350}, 60 };;
		p2_score_txt.color_filter = Hexeng::Color4::red;

		frame_outline = Hexeng::Renderer::Quad{ {0, -100}, {1536, 816}, nullptr };
		frame_outline.color = { "#055A64ff" };

		frame = Hexeng::Renderer::Quad{ {0, -100}, {1520, 800}, nullptr };
		frame.color = background.color;

		a = GameIcon("res/thumbnails/TronSnake.png");
		b = GameIcon("res/thumbnails/Tron.png", Tron::ungenerate_apple);
		c = GameIcon("res/thumbnails/TicTacToe.png");
		d = GameIcon("res/thumbnails/Connect4.png");
		e = GameIcon("res/thumbnails/HiddenConnect4.png", HiddenConnect4::on_game_begin);
		f = GameIcon("res/thumbnails/HiddenConnect4.png");
		g = GameIcon("res/thumbnails/coming_soon.png");
		h = GameIcon("res/thumbnails/coming_soon.png");
		i = GameIcon("res/thumbnails/coming_soon.png");
		j = GameIcon("res/thumbnails/coming_soon.png");
		k = GameIcon("res/thumbnails/coming_soon.png");
		l = GameIcon("res/thumbnails/coming_soon.png");
		m = GameIcon("res/thumbnails/coming_soon.png");
		n = GameIcon("res/thumbnails/coming_soon.png");
		o = GameIcon("res/thumbnails/coming_soon.png");
		p = GameIcon("res/thumbnails/coming_soon.png");
		q = GameIcon("res/thumbnails/coming_soon.png");
		r = GameIcon("res/thumbnails/coming_soon.png");
		s = GameIcon("res/thumbnails/coming_soon.png");
		t = GameIcon("res/thumbnails/coming_soon.png");

		for (auto& icon : GameIcon::grid)
		{
			game_selection_layer.meshes.push_back(&icon->mesh);
			game_selection_scene.events.push_back(&icon->btn);
		}

		escape_layer.enable = false;

		escape_fog = Hexeng::Renderer::Quad{{0, 0}, {99999, 99999}, nullptr};
		escape_fog.opacity = 0.8f;
		 escape_background = Hexeng::Renderer::Quad{ {0, 0}, {1000, 550}, nullptr };
		 escape_background.color = { "#051423ff" };
		escape_frame = Hexeng::Renderer::Quad{{0, 0}, {1075, 650}, &golden_frame_tex};

		auto escape_menu_switch = []() {
			escape_layer.enable = !escape_layer.enable;
			menu_btn.switch_on_off();
			save_btn.switch_on_off();
			exit_btn.switch_on_off();
			settings_btn.switch_on_off();
			for (auto icon : GameIcon::grid)
			{
				icon->btn.enable = !icon->btn.enable;
			}};

		auto ig_escape_menu_switch = []() {
			ig_escape_layer.enable = !ig_escape_layer.enable;
			game_selection_btn.switch_on_off();
			p1_surrender.switch_on_off();
			p2_surrender.switch_on_off();
			ig_settings_btn.switch_on_off(); };

		press_escape = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_ESCAPE,
			[escape_menu_switch, ig_escape_menu_switch]() {
				if (Hexeng::scene_id == 2)
				{
					escape_menu_switch();
				}
				else if (Hexeng::scene_id > 2 && !(ig_win_layer.enable))
				{
					ig_escape_menu_switch();
				}} };

		menu_btn = DefaultButton{ U"Menu", [escape_menu_switch]() {escape_menu_switch(); Hexeng::Scene::switch_scene(0); }, {0, 150}, 8.0f, escape_background.color };
		save_btn = DefaultButton{ U"Save", [escape_menu_switch]() { score_save.save("saves/scores.hxg_save"); escape_menu_switch(); } , {0, 50}, 8.0f, escape_background.color };
		exit_btn = DefaultButton{ U"Exit",[]() {settings_save.save("saves/settings.hxg_save"); Hexeng::exit = true; }, {0, -50}, 8.0f, escape_background.color };
		settings_btn = DefaultButton{ U"Settings",[]() {scene_calling_settings = 2; Hexeng::Scene::switch_scene(1); }, {0, -150}, 8.0f, escape_background.color };

		menu_btn.disable();
		save_btn.disable();
		exit_btn.disable();
		settings_btn.disable();

		game_selection_btn = DefaultButton{ U"Duel Selection",
			[ig_escape_menu_switch]()
			{ ig_escape_menu_switch(); Hexeng::Scene::switch_scene(2); if (on_game_end) { on_game_end(); } on_game_end = nullptr; }
		, {0, 150}, 8.0f, escape_background.color };
		p1_surrender = DefaultButton{ U"P1 Surrender", [ig_escape_menu_switch]() { ig_escape_menu_switch(); player2_wins(); } , {0, 50}, 8.0f, escape_background.color };
		p2_surrender = DefaultButton{ U"P2 Surrender", [ig_escape_menu_switch]() { ig_escape_menu_switch(); player1_wins(); } , {0, -50}, 8.0f, escape_background.color };
		ig_settings_btn = DefaultButton{ U"Settings",[]() { Hexeng::Scene::switch_scene(1); }, {0, -150}, 8.0f, escape_background.color };;

		game_selection_win = DefaultButton{ U"Duel Selection",
			[]() {
				ig_win_layer.enable = false;
				game_selection_win.disable();
				Hexeng::Scene::switch_scene(2);
				if (on_game_end) { on_game_end(); } on_game_end = nullptr; }, { 0, 0 }, 8.0f, escape_background.color };
		game_selection_win.disable();

		ig_escape_layer.enable = false;

		game_selection_btn.disable();
		p1_surrender.disable();
		p2_surrender.disable();
		ig_settings_btn.disable();

		ig_win_line = Hexeng::Renderer::Quad{ {0, 85}, {600, 4}, nullptr };
		ig_win_line.color = Hexeng::Color4::white;
		p1_wins = Hexeng::Text{ &language_switch, U"P1 Wins !", roboto_font, {0, 120}, 100 };
		p2_wins = Hexeng::Text{ &language_switch, U"P2 Wins !", roboto_font, {0, 120}, 100 };
		draw_text = Hexeng::Text{ &language_switch, U"Draw !", roboto_font, {0, 120}, 100 };

		ig_win_layer.enable = false;

		stop_game_evt = Hexeng::EventManager::EventGate
		{
			[]()
			{
				stop_game = ig_escape_layer.enable || ig_win_layer.enable;

			}, Hexeng::Range::GLOBAL
		};

	}, 3
};

bool stop_game = false;
Hexeng::EventManager::EventGate stop_game_evt;

std::function<void(void)> on_game_end = nullptr;

void player1_wins()
{
	player1_score++;
	update_scores();
	p1_wins.enable = true;
	p2_wins.enable = false;
	draw_text.enable = false;
	ig_win_layer.enable = true;
	game_selection_win.enable();
}

void player2_wins()
{
	player2_score++;
	update_scores();
	p1_wins.enable = false;
	p2_wins.enable = true;
	draw_text.enable = false;
	ig_win_layer.enable = true;
	game_selection_win.enable();
}

void draw()
{
	player1_score++;
	player2_score++;
	update_scores();
	p1_wins.enable = false;
	p2_wins.enable = false;
	draw_text.enable = true;
	ig_win_layer.enable = true;
	game_selection_win.enable();
}

void update_scores()
{
	Hexeng::Renderer::pending_actions.push_back([]() {
		p1_score_txt = Hexeng::Text{ U"P1 " + to_u32string(player1_score), roboto_font, {-700, 350}, 60 };
		p1_score_txt.color_filter = Hexeng::Color4::cyan;
		p2_score_txt = Hexeng::Text{ U"P2 " + to_u32string(player2_score), roboto_font, {700, 350}, 60 };
		p2_score_txt.color_filter = Hexeng::Color4::red;
		});
}

std::vector<GameIcon*> GameIcon::grid;

GameIcon::GameIcon(std::string filepath, std::function<void(void)> on_game_begin_p)
	: texture(filepath), on_game_begin(on_game_begin_p),
	mesh(Hexeng::Vec2<int>{-600 + 300 * (grid.size() % 5), 200 - (int)(200 * (grid.size() / 5))}, {250, 150}, &texture),
	btn(mesh.get_min(), mesh.get_max(),
		{
			{
				Hexeng::EventManager::ButtonEvent::HOVER, [this]()
				{
					mesh.scale = 1.5f;
					button_hover_snd.play();
					btn.min = mesh.position - Hexeng::Vec2<float>{ mesh.get_size() / 2 } * mesh.scale;
					btn.max = mesh.position + Hexeng::Vec2<float>{ mesh.get_size() / 2 } *mesh.scale;
				}
			},
			{
				Hexeng::EventManager::ButtonEvent::UNHOVER, [this]()
				{
					mesh.scale = 1.0f;
					btn.min = mesh.get_min();
					btn.max = mesh.get_max();
				}
			}
		}, GLFW_MOUSE_BUTTON_1, Hexeng::Range::LOCAL)
{
	int index = grid.size();

	if (filepath != "res/thumbnails/coming_soon.png")
	{
		btn.click = [this, index]()
		{
			button_click_snd.play();
			Hexeng::Scene::switch_scene(2 + index + 1);
			scene_calling_settings = 2 + index + 1;
			if (on_game_begin)
				on_game_begin();
		};
	}

	grid.push_back(this);
}

GameIcon::GameIcon(GameIcon&& other) noexcept
	: texture(std::move(other.texture)),
	mesh(std::move(other.mesh)),
	btn(std::move(other.btn)),
	on_game_begin(std::move(other.on_game_begin))
{
	mesh.texture = &texture;

	auto it = std::find(grid.begin(), grid.end(), &other);
	*it = this;

	int index = it - grid.begin();

	btn.hover = [this]
	{
		mesh.scale = 1.5f;
		button_hover_snd.play();
		btn.min = mesh.position - Hexeng::Vec2<float>{ mesh.get_size() / 2 } * mesh.scale;
		btn.max = mesh.position + Hexeng::Vec2<float>{ mesh.get_size() / 2 } * mesh.scale;
	};

	btn.unhover = [this]
	{
		mesh.scale = 1.0f;
		btn.min = mesh.get_min();
		btn.max = mesh.get_max();
	};

	if (btn.click)
	{
		btn.click = [this, index]()
		{
			button_click_snd.play();
			Hexeng::Scene::switch_scene(2 + index + 1);
			scene_calling_settings = 2 + index + 1;
			if (on_game_begin)
				on_game_begin();
		};
	}
}

GameIcon& GameIcon::operator=(GameIcon&& other) noexcept
{
	texture = std::move(other.texture);
	mesh = std::move(other.mesh);
	mesh.texture = &texture;
	btn = std::move(other.btn);
	on_game_begin = std::move(other.on_game_begin);

	auto it = std::find(grid.begin(), grid.end(), &other);
	*it = this;

	int index = it - grid.begin();

	btn.hover = [this]
	{
		mesh.scale = 1.5f;
		button_hover_snd.play();
		btn.min = mesh.position - Hexeng::Vec2<float>{ mesh.get_size() / 2 } *mesh.scale;
		btn.max = mesh.position + Hexeng::Vec2<float>{ mesh.get_size() / 2 } *mesh.scale;
	};

	btn.unhover = [this]
	{
		mesh.scale = 1.0f;
		btn.min = mesh.get_min();
		btn.max = mesh.get_max();
	};

	if (btn.click)
	{
		btn.click = [this, index]()
		{
			button_click_snd.play();
			Hexeng::Scene::switch_scene(2 + index + 1);
			scene_calling_settings = 2 + index + 1;
			if (on_game_begin)
				on_game_begin();
		};
	}

	return *this;
}

std::u32string to_u32string (int i)
{
	std::u32string output;
	std::string str = std::to_string(i);
	output.reserve(str.size());
	for (auto c : str)
		output.push_back(c);
	return output;
}
