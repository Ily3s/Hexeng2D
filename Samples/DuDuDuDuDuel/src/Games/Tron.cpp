#include "Renderer/BatchRenderer.hpp"

#include "../GameSelection.hpp"
#include "../Menu.hpp"
#include <random>

namespace Snake
{

	extern Hexeng::Renderer::TextureAtlas grid_atlas;

	extern Hexeng::Renderer::BatchingShader batching_shader_max;

	extern std::vector<Hexeng::Renderer::BatchInstance> batch_instances;

	extern std::vector<Hexeng::Renderer::BatchQuad> snake_grid;

	extern Hexeng::Renderer::Layer backgroud_layer;
	extern Hexeng::Renderer::Layer snake_layer;

	/**CONTROLS AND GAME LOGIC**/

	extern Hexeng::Vec2<int> grid_size;

	// P1
	extern Hexeng::Vec2<float> p1_velocity;
	extern Hexeng::Vec2<float> p1_position;
	extern Hexeng::Vec2<float> p1_previous_velocity;
	extern float p1_speed;
	extern int p1_boost_cd;
	extern int p1_boost_left;
	extern Hexeng::EventManager::KeyEvent w;
	extern Hexeng::EventManager::KeyEvent a;
	extern Hexeng::EventManager::KeyEvent s;
	extern Hexeng::EventManager::KeyEvent d;
	extern Hexeng::EventManager::KeyEvent l_shift;

	// P2
	extern Hexeng::Vec2<float> p2_velocity;
	extern Hexeng::Vec2<float> p2_position;
	extern Hexeng::Vec2<float> p2_previous_velocity;
	extern float p2_speed;
	extern int p2_boost_cd;
	extern int p2_boost_left;
	extern Hexeng::EventManager::KeyEvent up;
	extern Hexeng::EventManager::KeyEvent left;
	extern Hexeng::EventManager::KeyEvent down;
	extern Hexeng::EventManager::KeyEvent right;
	extern Hexeng::EventManager::KeyEvent r_shift;

	void reset_snake();

}

namespace Tron
{
	using namespace Snake;

	void ungenerate_apple()
	{
		for (auto& quad : Snake::snake_grid)
		{
			if (quad.color_filter == Hexeng::Color4::green)
				quad.color_filter = Hexeng::Color4::white;
		}
	}

	Hexeng::EventManager::Event tron_game_logic;

	/**************************/

	Hexeng::Scene tron_scene{ 4, {
		{Hexeng::SceneComponent::LAYERS, {&backgroud_layer, &snake_layer, &ig_escape_layer, &ig_win_layer}},
		{Hexeng::SceneComponent::EVENTS, {&w, &a, &s, &d, &up, &left, &down, &right, &l_shift, &r_shift, &tron_game_logic}}
	} };

	Hexeng::Renderer::ToBeInit init_tron
	{
		[]()
		{
			for (auto evt : ig_base_evts)
				tron_scene.events.push_back(evt);

			tron_game_logic = Hexeng::EventManager::Event
			{ []() {return !stop_game; },
				[]()
				{
					using namespace Snake;

					on_game_end = reset_snake;

					if (p1_previous_velocity == -p1_velocity)
						p1_velocity = p1_previous_velocity;

					if (p2_previous_velocity == -p2_velocity)
						p2_velocity = p2_previous_velocity;

					p1_previous_velocity = p1_velocity;
					p2_previous_velocity = p2_velocity;

					p1_position += p1_velocity * p1_speed;
					p2_position += p2_velocity * p2_speed;

					bool p1_out = p1_position.x < 0 || p1_position.x >= grid_size.x || p1_position.y < 0 || p1_position.y >= grid_size.y;
					bool p2_out = p2_position.x < 0 || p2_position.x >= grid_size.x || p2_position.y < 0 || p2_position.y >= grid_size.y;

					if (p1_out)
					{
						player2_wins();
						return;
					}

					if (p2_out)
					{
						player1_wins();
						return;
					}

					auto& p1_quad = snake_grid[(int)p1_position.x * grid_size.y + (int)p1_position.y];
					auto& p2_quad = snake_grid[(int)p2_position.x * grid_size.y + (int)p2_position.y];

					bool p1_lost = (Hexeng::Vec2<int>(p1_position) != Hexeng::Vec2<int>(p1_position - p1_velocity * p1_speed))
						&& p1_quad.color_filter != Hexeng::Color4::white && p1_quad.color_filter != Hexeng::Color4::green;
					bool p2_lost = (Hexeng::Vec2<int>(p2_position) != Hexeng::Vec2<int>(p2_position - p2_velocity * p2_speed)) &&
						p2_quad.color_filter != Hexeng::Color4::white && p2_quad.color_filter != Hexeng::Color4::green;

					if ((Hexeng::Vec2<int>{p1_position} == Hexeng::Vec2<int>{p2_position}) || (p1_lost && p2_lost))
					{
						draw();
						return;
					}

					if (p1_lost)
					{
						player2_wins();
						return;
					}

					if (p2_lost)
					{
						player1_wins();
						return;
					}

					if (p1_boost_cd)
						p1_boost_cd--;
					else
						p1_speed = 1.0f;

					if (p2_boost_cd)
						p2_boost_cd--;
					else
						p2_speed = 1.0f;

					p1_quad.color_filter = Hexeng::Color4::cyan;
					p2_quad.color_filter = Hexeng::Color4::red;

				},
				Hexeng::Range::LOCAL, 5
			};

			/*************************/

		}, 5
	};

}