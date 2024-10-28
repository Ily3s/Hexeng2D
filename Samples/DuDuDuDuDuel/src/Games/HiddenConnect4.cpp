#include "Renderer/BatchRenderer.hpp"

#include "../GameSelection.hpp"
#include "../Menu.hpp"

namespace Connect4
{
	extern Hexeng::Renderer::Layer game_layer;
	 
	extern Hexeng::Scene connect4_scene;
	 
	extern Hexeng::Renderer::TextureAtlas circle;
	extern Hexeng::Renderer::BatchInstance batch;
	 
	extern std::array<std::array<Hexeng::Renderer::BatchQuad, 6>, 7> grid;
	 
	extern std::array<int, 7> stacks;
	 
	extern Hexeng::Renderer::Texture grid_texture;
	extern Hexeng::Renderer::Quad grid_mesh;
	 
	extern bool turn;
	 
	extern int colomn;
	extern Hexeng::Renderer::BatchQuad pointer_quad;
	 
	extern Hexeng::EventManager::KeyPressEvent d, s, a;
	extern Hexeng::EventManager::KeyPressEvent dr, sr, ar;
	extern Hexeng::EventManager::KeyPressEvent right, down, left;
	extern Hexeng::EventManager::KeyPressEvent right_r, down_r, left_r;
	 
	extern std::array<std::array<Hexeng::Vec2<int>, 4>, 69> winning_configurations;
	 
	extern int timer;
	extern Hexeng::Text timer_mesh;
	extern Hexeng::EventManager::Event timer_evt;
}

namespace HiddenConnect4
{
	using namespace Connect4;

	Hexeng::Scene hidden_connect4_scene{ 7, {
		{Hexeng::SceneComponent::LAYERS, {&game_layer, &ig_escape_layer, &ig_win_layer}}
	 } };

	Hexeng::Renderer::ToBeInit init_hidden_connect4
	{
		[]()
		{
			for (auto evt : ig_base_evts)
				hidden_connect4_scene.events.push_back(evt);

			hidden_connect4_scene.events.push_back(&timer_evt);
		}, 8
	};

	void on_game_begin()
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				grid[i][j].color = Hexeng::Color4{0.0f, 0.0f, 0.0f, 0.0f};
			}
		}
	}
}
