#include "Renderer/BatchRenderer.hpp"

#include "../GameSelection.hpp"
#include "../Menu.hpp"
#include <random>

namespace Snake
{

	Hexeng::Renderer::TextureAtlas grid_atlas;

	Hexeng::Renderer::BatchingShader batching_shader_max;

	std::vector<Hexeng::Renderer::BatchInstance> batch_instances;

	std::vector<Hexeng::Renderer::BatchQuad> snake_grid;

	Hexeng::Renderer::Layer backgroud_layer;
	Hexeng::Renderer::Layer snake_layer{ {}, 0, Hexeng::Renderer::Position::ABSOLUTE };

	/**CONTROLS AND GAME LOGIC**/

	Hexeng::Vec2<int> grid_size{ 40, 40 };

	// P1
	int p1_score = 10;
	bool p1_stop = false;
	std::vector<Hexeng::Vec2<int>> p1_queue;
	Hexeng::Vec2<float> p1_velocity{ 0.5f, 0 };
	Hexeng::Vec2<float> p1_position{ -0.5f, (float)grid_size.y / 2 };
	Hexeng::Vec2<float> p1_previous_velocity{ 0, 0 };
	float p1_speed = 1.0f;
	int p1_boost_cd = 0;
	int p1_boost_left = 3;
	Hexeng::EventManager::KeyEvent w;
	Hexeng::EventManager::KeyEvent a;
	Hexeng::EventManager::KeyEvent s;
	Hexeng::EventManager::KeyEvent d;
	Hexeng::EventManager::KeyEvent l_shift;

	// P2
	int p2_score = 10;
	bool p2_stop = false;
	std::vector<Hexeng::Vec2<int>> p2_queue;
	Hexeng::Vec2<float> p2_velocity{ -0.5f, 0 };
	Hexeng::Vec2<float> p2_position{ (float)grid_size.x, (float)grid_size.y / 2 };
	Hexeng::Vec2<float> p2_previous_velocity{ 0, 0 };
	float p2_speed = 1.0f;
	int p2_boost_cd = 0;
	int p2_boost_left = 3;
	Hexeng::EventManager::KeyEvent up;
	Hexeng::EventManager::KeyEvent left;
	Hexeng::EventManager::KeyEvent down;
	Hexeng::EventManager::KeyEvent right;
	Hexeng::EventManager::KeyEvent r_shift;

	Hexeng::EventManager::Event snake_game_logic;

	/**************************/

	Hexeng::Scene snake_scene{ 3, {
		{Hexeng::SceneComponent::LAYERS, {&backgroud_layer, &snake_layer, &ig_escape_layer, &ig_win_layer}},
		{Hexeng::SceneComponent::EVENTS, {&w, &a, &s, &d, &up, &left, &down, &right, &l_shift, &r_shift, &snake_game_logic}}
	} };

	void generate_apple()
	{
		std::random_device rng_device;
		std::mt19937 rng{ rng_device() };
		std::uniform_int_distribution<std::mt19937::result_type> distribute{ 0, (unsigned int)(grid_size.x * grid_size.y - p1_queue.size() - p2_queue.size()) };
		int random_index = distribute(rng);

		std::vector<int> unavailible_indicies;
		unavailible_indicies.reserve(p1_queue.size() + p2_queue.size());

		for (auto& pos : p1_queue)
			unavailible_indicies.push_back((int)pos.x * grid_size.y + (int)pos.y);

		for (auto& pos : p2_queue)
			unavailible_indicies.push_back((int)pos.x * grid_size.y + (int)pos.y);

		std::sort(unavailible_indicies.begin(), unavailible_indicies.end());

		for (int i = 0; i < unavailible_indicies.size(); i++)
		{
			if (random_index < unavailible_indicies[i])
				break;

			random_index++;
		}

		snake_grid[random_index].color_filter = Hexeng::Color4::green;
	}

	void reset_snake()
	{
		p1_velocity = { 0.5f, 0 };
		p1_position = { -0.5f, (float)grid_size.y / 2 };
		p1_previous_velocity = { 0, 0 };
		p1_speed = 1.0f;
		p1_boost_cd = 0;
		p1_boost_left = 3;
		p1_stop = false;

		p2_velocity = { -0.5f, 0 };
		p2_position = { (float)grid_size.x, (float)grid_size.y / 2 };
		p2_previous_velocity = { 0, 0 };
		p2_speed = 1.0f;
		p2_boost_cd = 0;
		p2_boost_left = 3;
		p2_stop = false;

		for (int i = 0; i < grid_size.x * grid_size.y; i++)
			snake_grid[i].color_filter = Hexeng::Color4::white;

		for (int i = grid_size.x * grid_size.y; i < grid_size.x * grid_size.y + 3; i++)
			snake_grid[i].color = Hexeng::Color4::cyan;

		for (int i = grid_size.x * grid_size.y + 3; i < grid_size.x * grid_size.y + 6; i++)
			snake_grid[i].color = Hexeng::Color4::red;

		p1_score = 10;
		p2_score = 10;

		p1_queue.clear();
		p2_queue.clear();

		generate_apple();
	}

	void end_game()
	{
		if (p1_score > p2_score)
			player1_wins();
		else if (p1_score < p2_score)
			player2_wins();
		else
			draw();
	}

	Hexeng::Renderer::ToBeInit init_snake
	{
		[]()
		{

			size_t quads_per_batch = std::min((grid_size.x * grid_size.y + 6), Hexeng::Renderer::BatchInstance::get_max_quads());

			batching_shader_max = Hexeng::Renderer::BatchingShader{ quads_per_batch };

			int batches_nb = std::ceil((double)(grid_size.x * grid_size.y + 6) / quads_per_batch);
			batch_instances.reserve(batches_nb);

			for (int i = 0; i < batches_nb; i++) 
				batch_instances.emplace_back(Hexeng::Renderer::TALayout{{1,1}, {1, 1}}, &batching_shader_max);

			for (auto& bi : batch_instances)
				snake_layer.meshes.push_back(&bi);

			backgroud_layer = { {&background}, 0, Hexeng::Renderer::Position::ABSOLUTE };

			for (auto evt : ig_base_evts)
				snake_scene.events.push_back(evt);

			snake_grid.reserve((grid_size.x * grid_size.y) + 6);

			for (int i = 0; i < grid_size.x; i++)
			{
				for (int j = 0; j < grid_size.y; j++)
				{
					snake_grid.emplace_back((Hexeng::Renderer::BatchInstance*)snake_layer.meshes[(i * grid_size.y + j) / quads_per_batch],
						Hexeng::Vec2<int>{ 0, 0 }, Hexeng::Vec2<int>{i* (1080 / grid_size.x), (j* (1080 / grid_size.y))}, 27.0f);

					snake_grid.back().color = Hexeng::Color4::white;
				}
			}

			for (int i = 0; i < 3; i++)
			{
				snake_grid.emplace_back((Hexeng::Renderer::BatchInstance*)snake_layer.meshes[(grid_size.x * grid_size.y + i) / quads_per_batch],
					Hexeng::Vec2<int>{0, 0}, Hexeng::Vec2<int>{-300 + i * 60, 100}, 50.0f);
				snake_grid.back().color = Hexeng::Color4::cyan;
			}

			for (int i = 0; i < 3; i++)
			{
				snake_grid.emplace_back((Hexeng::Renderer::BatchInstance*)snake_layer.meshes[(grid_size.x * grid_size.y + 3 + i) / quads_per_batch],
					Hexeng::Vec2<int>{0, 0}, Hexeng::Vec2<int>{1300 - i * 60, 1000}, 50.0f);
				snake_grid.back().color = Hexeng::Color4::red;
			}

			for (auto& bi : batch_instances)
			{
				bi.construct_batch();
				bi.position = { -527, -527 };
			}

			/**CONTROLS AND GAME LOGIC**/

			// P1
			w = Hexeng::EventManager::KeyEvent{ GLFW_KEY_W, [] {p1_velocity = {0, 0.5f}; }, Hexeng::Range::LOCAL };
			a = Hexeng::EventManager::KeyEvent{ GLFW_KEY_A, [] {p1_velocity = {-0.5f, 0}; }, Hexeng::Range::LOCAL };
			s = Hexeng::EventManager::KeyEvent{ GLFW_KEY_S, [] {p1_velocity = {0, -0.5f}; }, Hexeng::Range::LOCAL };
			d = Hexeng::EventManager::KeyEvent{ GLFW_KEY_D, [] {p1_velocity = {0.5f, 0}; }, Hexeng::Range::LOCAL };
			l_shift = Hexeng::EventManager::KeyEvent{ GLFW_KEY_LEFT_SHIFT, [] {
				if (!p1_boost_cd && p1_boost_left)
				{
					snake_grid[snake_grid.size() - 3 - p1_boost_left].color = Hexeng::Color4::white;
					p1_boost_left--;
					p1_speed = 2.0f;
					p1_boost_cd = 10;
				}
			}, Hexeng::Range::LOCAL };

			// P2
			up = Hexeng::EventManager::KeyEvent{ GLFW_KEY_UP, [] {p2_velocity = {0, 0.5f}; }, Hexeng::Range::LOCAL };
			left = Hexeng::EventManager::KeyEvent{ GLFW_KEY_LEFT, [] {p2_velocity = {-0.5f, 0}; }, Hexeng::Range::LOCAL };
			down = Hexeng::EventManager::KeyEvent{ GLFW_KEY_DOWN, [] {p2_velocity = {0, -0.5f}; }, Hexeng::Range::LOCAL };
			right = Hexeng::EventManager::KeyEvent{ GLFW_KEY_RIGHT, [] {p2_velocity = {0.5f, 0}; }, Hexeng::Range::LOCAL };
			r_shift = Hexeng::EventManager::KeyEvent{ GLFW_KEY_RIGHT_SHIFT, [] {
				if (!p2_boost_cd && p2_boost_left)
				{
					snake_grid[snake_grid.size() - p2_boost_left].color = Hexeng::Color4::white;
					p2_boost_left--;
					p2_speed = 2.0f;
					p2_boost_cd = 10;
				}
			}, Hexeng::Range::LOCAL };

			generate_apple();

			snake_game_logic = Hexeng::EventManager::Event
			{ []() {return !stop_game; },
				[]()
				{
					on_game_end = reset_snake;

					if (p1_previous_velocity == -p1_velocity)
						p1_velocity = p1_previous_velocity;

					if (p2_previous_velocity == -p2_velocity)
						p2_velocity = p2_previous_velocity;

					p1_previous_velocity = p1_velocity;
					p2_previous_velocity = p2_velocity;

					if (!p1_stop)
						p1_position += p1_velocity * p1_speed;

					if (!p2_stop)
						p2_position += p2_velocity * p2_speed;

					if (!p1_stop && ((!p1_queue.size()) || Hexeng::Vec2<int>{p1_position} != Hexeng::Vec2<int>{p1_position - (p1_velocity * p1_speed)}))
						p1_queue.push_back(p1_position);
					if (p1_queue.size() > p1_score)
					{
						snake_grid[p1_queue.front().x * grid_size.y + p1_queue.front().y].color_filter = Hexeng::Color4::white;
						p1_queue.erase(p1_queue.begin());
					}

					if (!p2_stop && Hexeng::Vec2<int>{p2_position} != Hexeng::Vec2<int>{p2_position - (p2_velocity * p2_speed)})
						p2_queue.push_back(p2_position);
					if (p2_queue.size() > p2_score)
					{
						snake_grid[p2_queue.front().x * grid_size.y + p2_queue.front().y].color_filter = Hexeng::Color4::white;
						p2_queue.erase(p2_queue.begin());
					}

					bool p1_out = p1_position.x < 0 || p1_position.x >= grid_size.x || p1_position.y < 0 || p1_position.y >= grid_size.y;
					bool p2_out = p2_position.x < 0 || p2_position.x >= grid_size.x || p2_position.y < 0 || p2_position.y >= grid_size.y;

					if (p1_out && !p1_stop)
					{
						p1_stop = true;
						p1_position -= p1_velocity * p1_speed;
						if (p2_stop)
						{
							end_game();
							return;
						}
					}

					if (p2_out && !p2_stop)
					{
						p2_stop = true;
						p2_position -= p2_velocity * p2_speed;
						if (p1_stop)
						{
							end_game();
							return;
						}
					}

					auto& p1_quad = snake_grid[(int)p1_position.x * grid_size.y + (int)p1_position.y];
					auto& p2_quad = snake_grid[(int)p2_position.x * grid_size.y + (int)p2_position.y];

					bool p1_lost = (Hexeng::Vec2<int>(p1_position) != Hexeng::Vec2<int>(p1_position - p1_velocity * p1_speed))
						&& p1_quad.color_filter != Hexeng::Color4::white && p1_quad.color_filter != Hexeng::Color4::green;
					bool p2_lost = (Hexeng::Vec2<int>(p2_position) != Hexeng::Vec2<int>(p2_position - p2_velocity * p2_speed)) &&
						p2_quad.color_filter != Hexeng::Color4::white && p2_quad.color_filter != Hexeng::Color4::green;

					if ((Hexeng::Vec2<int>{p1_position} == Hexeng::Vec2<int>{p2_position}) || (p1_lost && p2_lost))
					{
						end_game();
						return;
					}

					if (p1_lost)
					{
						p1_stop = true;
						if (p2_stop)
						{
							end_game();
							return;
						}
					}

					if (p2_lost)
					{
						p2_stop = true;
						if (p1_stop)
						{
							end_game();
							return;
						}
					}

					if (p1_boost_cd)
						p1_boost_cd--;
					else
						p1_speed = 1.0f;

					if (p2_boost_cd)
						p2_boost_cd--;
					else
						p2_speed = 1.0f;

					if (p1_quad.color_filter == Hexeng::Color4::green)
					{
						p1_score += 5;
						generate_apple();
					}

					if (p2_quad.color_filter == Hexeng::Color4::green)
					{
						p2_score += 5;
						generate_apple();
					}

					p1_quad.color_filter = Hexeng::Color4::cyan;
					p2_quad.color_filter = Hexeng::Color4::red;

				},
				Hexeng::Range::LOCAL, 5
			};

			/*************************/

		}, 4
	};

}
