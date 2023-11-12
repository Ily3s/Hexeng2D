#ifndef BOARD_HPP
#define BOARD_HPP

#include "Hexeng.hpp"
#include "Renderer/Quad.hpp"
#include "Physics/Hibox.hpp"

#include <array>

class Player;

class Tile
{
public :

	Hexeng::Renderer::Quad mesh;

	Tile(int x, int y, uint8_t* data);
	Tile() = default;

	uint8_t id, var1, var2, var3;

	bool physics = false;

	int pulse = 0, weight = 0;
};

extern std::array<Tile, 256> board;

struct Behaviour
{
	std::function<void(Tile*, Player*)> walks{[](Tile*, Player*){}};
	std::function<void(Tile*, Player*)> leave{ [](Tile*, Player*) {} };
	std::function<void(Tile*, Player*)> press{ [](Tile*, Player*) {} };
	std::function<void(Tile*)> activate{ [](Tile*) {} };
	std::function<void(Tile*)> deactivate{ [](Tile*) {} };
	
	bool physics = false;

	bool multi_pulse = false;
	bool multi_weight = false;
};

extern std::array<Behaviour, 256> behaviours;

void build_board(uint8_t*);

extern std::array<Hexeng::Renderer::Texture, 256> textures;
extern std::array<Hexeng::Renderer::Texture, 256> b_textures;

extern Hexeng::Renderer::Layer board_layer;

inline Hexeng::Vec2<int> pos2coord(int x, int y)
{
	return Hexeng::Vec2<int>{x, y}*64 - 512 + 32;
}

extern uint8_t* map;

extern uint64_t timer;

extern std::chrono::steady_clock::time_point start;

std::u32string time_to_string(uint64_t time);

void win();

#endif // !BOARD_HPP