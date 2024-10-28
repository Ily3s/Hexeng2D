#include "Renderer/BatchRenderer.hpp"

#include "../GameSelection.hpp"
#include "../Menu.hpp"

namespace Connect4
{
    namespace hxg = Hexeng;
    namespace hxgrend = Hexeng::Renderer;

	hxgrend::Layer game_layer;

	hxg::Scene connect4_scene{ 6, {
		{hxg::SceneComponent::LAYERS, {&game_layer, &ig_escape_layer, &ig_win_layer}}
	} };

	hxgrend::TextureAtlas circle;
	hxgrend::BatchInstance batch;

	std::array<std::array<hxgrend::BatchQuad, 6>, 7> grid;

	std::array<int, 7> stacks;

	hxgrend::Texture grid_texture;
	hxgrend::Quad grid_mesh;

	bool turn = 1;

	int colomn = 3;
	hxgrend::BatchQuad pointer_quad;

	hxg::EventManager::KeyPressEvent d, s, a;
	hxg::EventManager::KeyPressEvent dr, sr, ar;
	hxg::EventManager::KeyPressEvent right, down, left;
	hxg::EventManager::KeyPressEvent right_r, down_r, left_r;

	std::array<std::array<hxg::Vec2<int>, 4>, 69> winning_configurations;

	int timer = 5;
	hxg::Text timer_mesh;
	hxg::EventManager::Event timer_evt;

	void go_middle();

	void reset_game()
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				grid[i][j].opacity = 0.0f;
				grid[i][j].color = hxg::Color4{ 1.0f, 1.0f, 1.0f, 0.0f };
			}
			stacks[i] = 0;
		}

		go_middle();

		timer = 5;
		hxgrend::pending_actions.push_back([]() {
			timer_mesh = hxg::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 }; });
	}

	void go_left()
	{
		for (int i = colomn - 1; i >= 0; i--)
		{
			if (stacks[i] < 6)
			{
				pointer_quad.position.x -= 123 * (colomn - i);
				colomn = i;
				return;
			}
		}
	}

	void go_right()
	{
		for (int i = colomn + 1; i < 7; i++)
		{
			if (stacks[i] < 6)
			{
				pointer_quad.position.x += 123 * (i - colomn);
				colomn = i;
				return;
			}
		}
	}

	void go_middle()
	{
		for (int i = 3; i < 7;)
		{
			if (stacks[i] < 6)
			{
				pointer_quad.position.x = -370 + 123 * i;
				colomn = i;
				return;
			}
			if (i <= 3)
				i = 3 + (3 - i + 1);
			else
				i = 3 - (i - 3);
		}
	}

	void check_for_wins()
	{
		for (auto& config : winning_configurations)
		{
			bool p1_wins = true, p2_wins = true;
			for (auto& pos : config)
			{
				p1_wins = p1_wins && (grid[pos.x][pos.y].color_filter == hxg::Color4::cyan);
				p2_wins = p2_wins && (grid[pos.x][pos.y].color_filter == hxg::Color4::red);
			}
			if (p1_wins)
			{
				player1_wins();
				return;
			}
			if (p2_wins)
			{
				player2_wins();
				return;
			}
		}

		bool all_filled = true;
		for (int i = 0; i < 7; i++)
			all_filled = all_filled && (stacks[i] == 6);
		if (all_filled)
			draw();
	}

	hxgrend::ToBeInit init_connect4
	{
		[]()
		{
			for (auto evt : ig_base_evts)
				connect4_scene.events.push_back(evt);

			grid_texture = hxgrend::Texture{"res/games/connect4/grid.png"};
			grid_mesh = hxgrend::Quad{{0, -100}, 1.1f, &grid_texture};

			circle = hxgrend::TextureAtlas{ "res/games/connect4/circle.png" };

			batch = hxgrend::BatchInstance{ hxgrend::TALayout{ {100, 100}, {1, 1} } };

			timer_mesh = hxg::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 };

			hxg::Vec2<int> bottom_left = { -370, -435 };

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					grid[i][j] = hxgrend::BatchQuad{ &batch, hxg::Vec2<int>{0, 0}, bottom_left + hxg::Vec2<int>{i * 123, j * 135}, 1.1f };
					grid[i][j].opacity = 0.0f;
					grid[i][j].color = hxg::Color4{ 1.0f, 1.0f, 1.0f, 0.0f };
				}
				stacks[i] = 0;
			}

			pointer_quad = hxgrend::BatchQuad{ &batch, hxg::Vec2<int>{0, 0}, {(float)bottom_left.x + 3 * 123, (float)bottom_left.y + 6 * 135}, 1.1f };
			pointer_quad.color_filter = hxg::Color4::cyan;
			pointer_quad.color = hxg::Color4{1.0f, 1.0f, 1.0f, 0.0f};

			batch.construct_batch();

			game_layer = hxgrend::Layer{ {&background, &grid_mesh, &batch, &timer_mesh}, 0, hxgrend::Position::ABSOLUTE };

			auto right_action = []() {
				if ((hxg::scene_id == 6 || hxg::scene_id == 7) && !stop_game)
				{
					on_game_end = reset_game;
					go_right();
				}
			};

			auto left_action = []() {
				if ((hxg::scene_id == 6 || hxg::scene_id == 7) && !stop_game)
				{
					on_game_end = reset_game;
					go_left();
				}
			};

			auto down_action = []() {
				if ((hxg::scene_id == 6 || hxg::scene_id == 7) && !stop_game)
				{
					on_game_end = reset_game;
					grid[colomn][stacks[colomn]].color_filter = pointer_quad.color_filter;
					stacks[colomn]++;
					if (turn)
						pointer_quad.color_filter = hxg::Color4::red;
					else
						pointer_quad.color_filter = hxg::Color4::cyan;
					turn = !turn;

					timer = 5;
					hxgrend::pending_actions.push_back([]() {
						timer_mesh = hxg::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 }; });

					check_for_wins();

					go_middle();
				}
			};

			d = hxg::EventManager::KeyPressEvent{ GLFW_KEY_D, right_action };
			a = hxg::EventManager::KeyPressEvent{ GLFW_KEY_A, left_action };
			s = hxg::EventManager::KeyPressEvent{ GLFW_KEY_S, down_action };

			dr = hxg::EventManager::KeyPressEvent{ GLFW_KEY_D, right_action, GLFW_REPEAT };
			ar = hxg::EventManager::KeyPressEvent{ GLFW_KEY_A, left_action, GLFW_REPEAT };
			sr = hxg::EventManager::KeyPressEvent{ GLFW_KEY_S, down_action, GLFW_REPEAT };

			right = hxg::EventManager::KeyPressEvent{ GLFW_KEY_RIGHT, right_action };
			left = hxg::EventManager::KeyPressEvent{ GLFW_KEY_LEFT, left_action };
			down = hxg::EventManager::KeyPressEvent{ GLFW_KEY_DOWN, down_action };

			right_r = hxg::EventManager::KeyPressEvent{ GLFW_KEY_RIGHT, right_action, GLFW_REPEAT };
			left_r = hxg::EventManager::KeyPressEvent{ GLFW_KEY_LEFT, left_action, GLFW_REPEAT };
			down_r = hxg::EventManager::KeyPressEvent{ GLFW_KEY_DOWN, down_action, GLFW_REPEAT };

			timer_evt = hxg::EventManager::Event{ []() {return !stop_game; }, [down_action]() {
				on_game_end = reset_game;
				if (timer)
					timer--;
				else
				{
					down_action();
					timer = 5;
				}
				hxgrend::pending_actions.push_back([]() {
					timer_mesh = hxg::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 }; });
			}, hxg::Range::LOCAL, 100 };

			connect4_scene.events.push_back(&timer_evt);

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					winning_configurations[i * 6 + j] = { hxg::Vec2<int>{i, j}, {i + 1, j}, {i + 2, j}, {i + 3, j} };
				}
			}

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					winning_configurations[24 + i * 3 + j] = { hxg::Vec2<int>{i, j}, {i, j + 1}, {i, j + 2}, {i, j + 3} };
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					winning_configurations[45 + i * 3 + j] = { hxg::Vec2<int>{i, j}, {i + 1, j + 1}, {i + 2, j + 2}, {i + 3, j + 3} };
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 5; j >= 3; j--)
				{
					winning_configurations[57 + i * 3 + (5 - j)] = { hxg::Vec2<int>{i, j}, {i + 1, j - 1}, {i + 2, j - 2}, {i + 3, j - 3} };
				}
			}

		}, 7
	};
}
