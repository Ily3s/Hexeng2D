#ifndef GAME_SELECTION_HPP
#define GAME_SELECTION_HPP

#include <functional>

#include "Scene.hpp"
#include "EventManager/Button.hpp"
#include "SaveSystem.hpp"

#include "Resources.hpp"

std::u32string to_u32string(int i);

extern Hexeng::Scene game_selection_scene;

extern int player1_score, player2_score;
extern void update_scores();

extern Hexeng::Renderer::Layer ig_win_layer;
extern Hexeng::Renderer::Layer ig_escape_layer;
extern std::vector<Hexeng::EventManager::Event*> ig_base_evts;

void player1_wins();
void player2_wins();
void draw();

extern std::function<void(void)> on_game_end;

extern Hexeng::SaveFile score_save;

extern bool stop_game;
extern Hexeng::EventManager::EventGate stop_game_evt;

class GameIcon
{
public :

	GameIcon(std::string filepath, std::function<void(void)> on_game_begin = nullptr);

	GameIcon() = default;

	GameIcon(GameIcon&&) noexcept;
	GameIcon& operator=(GameIcon&&) noexcept;

	Hexeng::Renderer::Texture texture;
	Hexeng::Renderer::Quad mesh;
	Hexeng::EventManager::Button btn;

	static std::vector<GameIcon*> grid;

	std::function<void(void)> on_game_begin = nullptr;
};

#endif