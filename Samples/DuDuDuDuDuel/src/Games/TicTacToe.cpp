#include "../GameSelection.hpp"
#include "../Menu.hpp"

namespace TicTacToe
{
	Hexeng::Renderer::Texture grid_tex;
	Hexeng::Renderer::Texture cross;
	Hexeng::Renderer::Texture circle;

	std::array<std::array<Hexeng::Renderer::Quad, 3>, 3> game_mesh_grid;
	std::array<std::array<Hexeng::EventManager::Button, 3>, 3> game_evt_grid;

	Hexeng::Renderer::Layer game_layer;

	Hexeng::Scene tictactoe_scene{ 5, {
		{Hexeng::SceneComponent::LAYERS, {&game_layer, &ig_escape_layer, &ig_win_layer}}
	} };

	Hexeng::Renderer::Quad grid_mesh;

	Hexeng::Renderer::Quad whos_turn;

	bool turn = 1;

	void reset_game()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				game_mesh_grid[i][j].enable = false;
			}
		}
	}

	void check_for_wins()
	{
		for (int i = 0; i < 3; i++)
		{
			bool p1_win = true, p2_win = true;
			for (int j = 0; j < 3; j++)
			{
				p1_win = p1_win && (game_mesh_grid[i][j].texture == &circle) && (game_mesh_grid[i][j].enable);
				p2_win = p2_win && (game_mesh_grid[i][j].texture == &cross) && (game_mesh_grid[i][j].enable);
			}
			if (p1_win)
			{
				player1_wins();
				return;
			}
			if (p2_win)
			{
				player2_wins();
				return;
			}
		}

		for (int j = 0; j < 3; j++)
		{
			bool p1_win = true, p2_win = true;
			for (int i = 0; i < 3; i++)
			{
				p1_win = p1_win && (game_mesh_grid[i][j].texture == &circle) && (game_mesh_grid[i][j].enable);
				p2_win = p2_win && (game_mesh_grid[i][j].texture == &cross) && (game_mesh_grid[i][j].enable);
			}
			if (p1_win)
			{
				player1_wins();
				return;
			}
			if (p2_win)
			{
				player2_wins();
				return;
			}
		}

		if (game_mesh_grid[0][0].enable && game_mesh_grid[1][1].enable && game_mesh_grid[2][2].enable)
		{
			if ((game_mesh_grid[0][0].texture == &circle) && (game_mesh_grid[1][1].texture == &circle) && (game_mesh_grid[2][2].texture == &circle))
			{
				player1_wins();
				return;
			}

			if ((game_mesh_grid[0][0].texture == &cross) && (game_mesh_grid[1][1].texture == &cross) && (game_mesh_grid[2][2].texture == &cross))
			{
				player2_wins();
				return;
			}
		}

		if (game_mesh_grid[0][2].enable && game_mesh_grid[1][1].enable && game_mesh_grid[2][0].enable)
		{
			if ((game_mesh_grid[0][2].texture == &circle) && (game_mesh_grid[1][1].texture == &circle) && (game_mesh_grid[2][0].texture == &circle))
			{
				player1_wins();
				return;
			}

			if ((game_mesh_grid[0][2].texture == &cross) && (game_mesh_grid[1][1].texture == &cross) && (game_mesh_grid[2][0].texture == &cross))
			{
				player2_wins();
				return;
			}
		}

		bool game_filled = true;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				game_filled = game_filled && game_mesh_grid[i][j].enable;
			}
		}
		if (game_filled)
			draw();
	}

	Hexeng::Renderer::ToBeInit init_tic_tac_toe
	{
		[]()
		{
			for (auto evt : ig_base_evts)
				tictactoe_scene.events.push_back(evt);

			grid_tex = Hexeng::Renderer::Texture{"res/games/tictactoe/grid.png"};
			cross = Hexeng::Renderer::Texture{ "res/games/tictactoe/cross.png" };
			circle = Hexeng::Renderer::Texture{ "res/games/tictactoe/circle.png" };

			grid_mesh = Hexeng::Renderer::Quad{{0, 0}, 1.0f, &grid_tex};

			whos_turn = Hexeng::Renderer::Quad{ {-700, -350}, 0.5f, &circle };

			game_layer = Hexeng::Renderer::Layer{ {&background, &grid_mesh, &whos_turn}, 0, Hexeng::Renderer::Position::ABSOLUTE };

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					game_mesh_grid[i][j] = Hexeng::Renderer::Quad{ {-250 + (float)i * 250, -250 + (float)j * 250 }, 1.0f, &circle };
					game_mesh_grid[i][j].enable = false;
					game_layer.meshes.push_back(&(game_mesh_grid[i][j]));

					game_evt_grid[i][j] = Hexeng::EventManager::Button{
						{game_mesh_grid[i][j].position - (circle.get_size() / 2)}, {game_mesh_grid[i][j].position + (circle.get_size() / 2)},
						{{Hexeng::EventManager::ButtonEvent::CLICK, [i, j]()
						{
							on_game_end = reset_game;

							if (!(game_mesh_grid[i][j].enable || stop_game))
							{
								if (turn)
								{
									game_mesh_grid[i][j].texture = &circle;
									whos_turn.texture = &cross;
								}
								else
								{
									game_mesh_grid[i][j].texture = &cross;
									whos_turn.texture = &circle;
								}

								game_mesh_grid[i][j].enable = true;
								turn = !turn;

								check_for_wins();
							}
						}}}, GLFW_MOUSE_BUTTON_1, Hexeng::Range::LOCAL
					};

					tictactoe_scene.events.push_back(&(game_evt_grid[i][j]));
				}
			}

		}, 6
	};
}
