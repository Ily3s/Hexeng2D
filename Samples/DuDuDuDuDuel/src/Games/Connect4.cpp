#include "Renderer/BatchRenderer.hpp"

#include "../GameSelection.hpp"
#include "../Menu.hpp"

namespace Connect4
{
	Hexeng::Renderer::Layer game_layer;

	Hexeng::Scene connect4_scene{ 6, {
		{Hexeng::SceneComponent::LAYERS, {&game_layer, &ig_escape_layer, &ig_win_layer}}
	} };

	Hexeng::Renderer::TextureAtlas circle;
	Hexeng::Renderer::BatchInstance batch;

	std::array<std::array<Hexeng::Renderer::BatchQuad, 6>, 7> grid;

	std::array<int, 7> stacks;

	Hexeng::Renderer::Texture grid_texture;
	Hexeng::Renderer::Quad grid_mesh;

	bool turn = 1;

	int colomn = 3;
	Hexeng::Renderer::BatchQuad pointer_quad;

	Hexeng::EventManager::KeyPressEvent d, s, a;
	Hexeng::EventManager::KeyPressEvent dr, sr, ar;
	Hexeng::EventManager::KeyPressEvent right, down, left;
	Hexeng::EventManager::KeyPressEvent right_r, down_r, left_r;

	std::array<std::array<Hexeng::Vec2<int>, 4>, 69> winning_configurations;

	int timer = 5;
	Hexeng::Text timer_mesh;
	Hexeng::EventManager::Event timer_evt;

	void go_middle();

	void reset_game()
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				grid[i][j].opacity = 0.0f;
				grid[i][j].color = Hexeng::Color4{ 1.0f, 1.0f, 1.0f, 0.0f };
			}
			stacks[i] = 0;
		}

		go_middle();

		timer = 5;
		Hexeng::Renderer::pending_actions.push_back([]() {
			timer_mesh = Hexeng::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 }; });
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
				p1_wins = p1_wins && (grid[pos.x][pos.y].color_filter == Hexeng::Color4::cyan);
				p2_wins = p2_wins && (grid[pos.x][pos.y].color_filter == Hexeng::Color4::red);
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

	Hexeng::Renderer::ToBeInit init_connect4
	{
		[]()
		{
			for (auto evt : ig_base_evts)
				connect4_scene.events.push_back(evt);

			grid_texture = Hexeng::Renderer::Texture{"res/games/connect4/grid.png"};
			grid_mesh = Hexeng::Renderer::Quad{{0, -100}, 1.1f, &grid_texture};

			circle = Hexeng::Renderer::TextureAtlas{ "res/games/connect4/circle.png", {100, 100} };

			batch = Hexeng::Renderer::BatchInstance{ &circle };

			timer_mesh = Hexeng::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 };

			Hexeng::Vec2<int> bottom_left = { -370, -435 };

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					grid[i][j] = Hexeng::Renderer::BatchQuad{ &batch, {0, 0}, bottom_left + Hexeng::Vec2<int>{i * 123, j * 135}, 1.1f };
					grid[i][j].opacity = 0.0f;
					grid[i][j].color = Hexeng::Color4{ 1.0f, 1.0f, 1.0f, 0.0f };
				}
				stacks[i] = 0;
			}

			pointer_quad = Hexeng::Renderer::BatchQuad{ &batch, {0, 0}, {bottom_left.x + 3 * 123, bottom_left.y + 6 * 135}, 1.1f };
			pointer_quad.color_filter = Hexeng::Color4::cyan;
			pointer_quad.color = Hexeng::Color4{1.0f, 1.0f, 1.0f, 0.0f};

			batch.construct_batch();

			game_layer = Hexeng::Renderer::Layer{ {&background, &grid_mesh, &batch, &timer_mesh}, 0, Hexeng::Renderer::Position::ABSOLUTE };

			auto right_action = []() {
				if ((Hexeng::scene_id == 6 || Hexeng::scene_id == 7) && !stop_game)
				{
					on_game_end = reset_game;
					go_right();
				}
			};

			auto left_action = []() {
				if ((Hexeng::scene_id == 6 || Hexeng::scene_id == 7) && !stop_game)
				{
					on_game_end = reset_game;
					go_left();
				}
			};

			auto down_action = []() {
				if ((Hexeng::scene_id == 6 || Hexeng::scene_id == 7) && !stop_game)
				{
					on_game_end = reset_game;
					grid[colomn][stacks[colomn]].color_filter = pointer_quad.color_filter;
					stacks[colomn]++;
					if (turn)
						pointer_quad.color_filter = Hexeng::Color4::red;
					else
						pointer_quad.color_filter = Hexeng::Color4::cyan;
					turn = !turn;

					timer = 5;
					Hexeng::Renderer::pending_actions.push_back([]() {
						timer_mesh = Hexeng::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 }; });

					check_for_wins();

					go_middle();
				}
			};

			d = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_D, right_action };
			a = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_A, left_action };
			s = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_S, down_action };

			dr = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_D, right_action, GLFW_REPEAT };
			ar = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_A, left_action, GLFW_REPEAT };
			sr = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_S, down_action, GLFW_REPEAT };

			right = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_RIGHT, right_action };
			left = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_LEFT, left_action };
			down = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_DOWN, down_action };

			right_r = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_RIGHT, right_action, GLFW_REPEAT };
			left_r = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_LEFT, left_action, GLFW_REPEAT };
			down_r = Hexeng::EventManager::KeyPressEvent{ GLFW_KEY_DOWN, down_action, GLFW_REPEAT };

			timer_evt = Hexeng::EventManager::Event{ []() {return !stop_game; }, [down_action]() {
				on_game_end = reset_game;
				if (timer)
					timer--;
				else
				{
					down_action();
					timer = 5;
				}
				Hexeng::Renderer::pending_actions.push_back([]() {
					timer_mesh = Hexeng::Text{ to_u32string(timer), roboto_font, {700, 300}, 75 }; });
			}, Hexeng::Range::LOCAL, 100 };

			connect4_scene.events.push_back(&timer_evt);

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					winning_configurations[i * 6 + j] = { Hexeng::Vec2<int>{i, j}, {i + 1, j}, {i + 2, j}, {i + 3, j} };
				}
			}

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					winning_configurations[24 + i * 3 + j] = { Hexeng::Vec2<int>{i, j}, {i, j + 1}, {i, j + 2}, {i, j + 3} };
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					winning_configurations[45 + i * 3 + j] = { Hexeng::Vec2<int>{i, j}, {i + 1, j + 1}, {i + 2, j + 2}, {i + 3, j + 3} };
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 5; j >= 3; j--)
				{
					winning_configurations[57 + i * 3 + (5 - j)] = { Hexeng::Vec2<int>{i, j}, {i + 1, j - 1}, {i + 2, j - 2}, {i + 3, j - 3} };
				}
			}

		}, 7
	};
}
