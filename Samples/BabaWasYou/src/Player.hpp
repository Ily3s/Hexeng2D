#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Board.hpp"

#include "EventManager/InputEvent.hpp"
#include "Audio/Audio.hpp"
#include "Text.hpp"

enum MoveDirection
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	INTERACT
};

struct Move
{
	std::chrono::steady_clock::duration wait_for;
	MoveDirection direction;
};

extern Hexeng::Renderer::Layer player_layer;
extern Hexeng::Renderer::Layer UI_layer;

class Player
{
public :

	~Player();

	Player(Player&&) noexcept = default;
	Player& operator=(Player&&) noexcept = default;

	uint8_t pos;

	Hexeng::Renderer::Quad mesh;

	static void init_player(uint8_t);

	static Player* get();

	void move(bool, int, Hexeng::Vec2<int>, int);

	std::vector<Move> registered_moves;

	std::chrono::steady_clock::time_point last_move;

	static std::vector<Player> storage;

	std::thread replay_thread;

	void replay();

	bool end_replay = false;

	void press();

private :

	friend struct std::_Default_allocator_traits<std::vector<Player>::allocator_type>;

	Player(uint8_t pos);

	static Hexeng::Renderer::Texture s_texture;

	static Hexeng::Renderer::ToBeInit Player::s_init_res;

};

extern Hexeng::EventManager::KeyPressEvent W;
extern Hexeng::EventManager::KeyPressEvent A;
extern Hexeng::EventManager::KeyPressEvent S;
extern Hexeng::EventManager::KeyPressEvent D;

extern Hexeng::EventManager::KeyPressEvent X;
extern Hexeng::EventManager::KeyPressEvent E;

extern Hexeng::Audio::Sound move_sound;
extern Hexeng::Audio::Sound press_x_fail;
extern Hexeng::Audio::Sound press_x_success;
extern Hexeng::Audio::Sound loot_sound;

extern int gems_count;
extern Hexeng::Renderer::Texture gem_texture;
extern Hexeng::Renderer::Quad gem_mesh;
extern Hexeng::Text gem_display;

extern Hexeng::Font roboto;

void update_gems_display();

#endif