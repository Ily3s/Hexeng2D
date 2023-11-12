#include "Player.hpp"
#include "Menu.hpp"

#include "Hexeng.hpp"
#include "Scene.hpp"

std::vector<Player> Player::storage;
Hexeng::Renderer::Texture Player::s_texture;

Hexeng::Audio::Sound move_sound;
Hexeng::Audio::Sound press_x_fail;
Hexeng::Audio::Sound press_x_success;
Hexeng::Audio::Sound loot_sound;

int gems_count = 0;
Hexeng::Renderer::Texture gem_texture;
Hexeng::Renderer::Quad gem_mesh;
Hexeng::Text gem_display;

Hexeng::Font roboto;

Hexeng::Renderer::ToBeInit init_gem_count{ []()
	{
		gem_texture = {"res/gem.png", {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}} };
		gem_mesh = Hexeng::Renderer::Quad{ {-800, 410}, {164, 164}, &gem_texture };
		roboto = Hexeng::Font{ "res/Roboto-Regular.ttf" };
		gem_display = Hexeng::Text{ U"0", roboto, {-710, 400}, 80, Hexeng::HorizontalAlign::LEFT };
		UI_layer.meshes.push_back(&gem_mesh);
		UI_layer.meshes.push_back(&gem_display);
	}, 2
};

std::mutex m;
std::condition_variable cv;

Hexeng::Renderer::Layer player_layer
{
	{}, 1, Hexeng::Renderer::Position::ABSOLUTE
};

Hexeng::Renderer::Layer UI_layer
{
	{}, 1, Hexeng::Renderer::Position::ABSOLUTE
};

Hexeng::Renderer::ToBeInit Player::s_init_res{ []()
	{
		s_texture = Hexeng::Renderer::Texture{"res/player.png", {{Hexeng::Renderer::TexSett::MAG_FILTER, GL_NEAREST}}};

		move_sound = { "res/move.wav" };
		press_x_fail = { "res/press_x_fail.wav" };
		press_x_success = { "res/press_x_success.wav" };
		loot_sound = { "res/loot.wav" };
	}
};

void Player::init_player(uint8_t pos)
{
	storage.reserve(256);
	storage.emplace_back(pos);
}

Player::Player(uint8_t pos)
	: pos(pos),
	mesh(pos2coord(pos % 16, pos / 16), { 64, 64 }, &s_texture),
	last_move(std::chrono::high_resolution_clock::now())
{
	player_layer.meshes.push_back(&mesh);

	board[pos].weight++;
	if (board[pos].weight == 1 || behaviours[board[pos].id].multi_weight)
		behaviours[board[pos].id].walks(&board[pos], this);

	Hexeng::Renderer::ToBeDelete(this, [this]() { this->~Player(); });
}

Player::~Player()
{
	if (replay_thread.joinable())
	{
		std::unique_lock lk{ m };
		end_replay = true;
		lk.unlock();
		cv.notify_one();
		replay_thread.join();
	}
	auto it = std::find(player_layer.meshes.begin(), player_layer.meshes.end(), &mesh);
	if (it != player_layer.meshes.end())
		player_layer.meshes.erase(it);
}

Player* Player::get()
{
	return &storage.back();
}

Hexeng::EventManager::KeyPressEvent W;
Hexeng::EventManager::KeyPressEvent A;
Hexeng::EventManager::KeyPressEvent S;
Hexeng::EventManager::KeyPressEvent D;

void Player::move(bool move_authorized, int logical_move, Hexeng::Vec2<int> mesh_move, int rota)
{
	mesh.rotation = rota;
	if (move_authorized && !board[pos + logical_move].physics)
	{
		move_sound.play();
		board[pos].weight--;
		if (!board[pos].weight || behaviours[board[pos].id].multi_weight)
			behaviours[board[pos].id].leave(&board[pos], this);
		pos += logical_move;
		mesh.position += mesh_move;
		board[pos].weight++;
		if (board[pos].weight == 1 || behaviours[board[pos].id].multi_weight)
			behaviours[board[pos].id].walks(&board[pos], this);
	}
}

Hexeng::EventManager::KeyPressEvent X;
Hexeng::EventManager::KeyPressEvent E;

Hexeng::EventManager::KeyPressEvent ESCAPE;
Hexeng::EventManager::KeyPressEvent R;

Hexeng::Renderer::ToBeInit init_controls{ []()
	{
		W = { 'W', []()
			{
				if (Hexeng::scene_id != 2)
					 return;

				Player::get()->move(Player::get()->pos < 240, 16, {0, 64}, 0);
				auto now = std::chrono::high_resolution_clock::now();
				Player::get()->registered_moves.push_back({ now - Player::get()->last_move, MoveDirection::UP });
				Player::get()->last_move = now;
			}
		};

		A = { 'A', []()
			{
				if (Hexeng::scene_id != 2)
					 return;

				Player::get()->move(Player::get()->pos % 16 > 0, -1, {-64, 0}, -90);
				auto now = std::chrono::high_resolution_clock::now();
				Player::get()->registered_moves.push_back({ now - Player::get()->last_move, MoveDirection::LEFT });
				Player::get()->last_move = now;
			}
		};

		 S = { 'S', []()
			{
				 if (Hexeng::scene_id != 2)
					 return;

				Player::get()->move(Player::get()->pos > 15, -16, {0, -64}, 180);
				auto now = std::chrono::high_resolution_clock::now();
				Player::get()->registered_moves.push_back({ now - Player::get()->last_move, MoveDirection::DOWN });
				Player::get()->last_move = now;
			}
		 };

		 D = { 'D', []()
			{
				 if (Hexeng::scene_id != 2)
					 return;

				Player::get()->move(Player::get()->pos % 16 < 15, 1, {64, 0}, 90);
				auto now = std::chrono::high_resolution_clock::now();
				Player::get()->registered_moves.push_back({ now - Player::get()->last_move, MoveDirection::RIGHT });
				Player::get()->last_move = now;
			}
		 };

		 X = { 'X', []()
			{
				 if (Hexeng::scene_id != 2)
					 return;

				 if (!gems_count)
				 {
					 press_x_fail.play();
					 // todo : animation
					 return;
				 }
				 press_x_success.play();

				 for (int i = 0; i < Player::storage.size() - 1; i++)
				 {
					 board[Player::storage[i].pos].weight--;
					 if (!board[Player::storage[i].pos].weight || behaviours[board[Player::storage[i].pos].id].multi_weight)
						behaviours[board[Player::storage[i].pos].id].leave(&board[Player::storage[i].pos], &Player::storage[i]);
					 std::unique_lock lk{ m };
					 Player::storage[i].end_replay = true;
					 lk.unlock();
					 cv.notify_one();
					 cv.wait(lk, [i]() { return Player::storage[i].replay_thread.joinable(); });
					 Player::storage[i].replay_thread.join();
				 }
				 
				 Hexeng::Renderer::pending_actions.push_back([]() {
					 build_board(map);
					 for (auto& player : Player::storage)
					 {
						 player.pos = Player::get()->pos;
						 board[player.pos].weight++;
						 if (board[player.pos].weight == 1 || behaviours[board[player.pos].id].multi_weight)
							 behaviours[board[player.pos].id].walks(&board[player.pos], &player);
						 player.mesh.rotation = Player::get()->mesh.rotation;
						 player.mesh.position = Player::get()->mesh.position;
					 }
					 for (int i = 0; i < Player::storage.size() - 1; i++)
					 {
						 Player::storage[i].end_replay = false;
						 Player::storage[i].replay_thread = std::thread([i]() {Player::storage[i].replay(); });
					 }
					 gems_count--;
					 update_gems_display();
				 });
			}
		 };

		 E = { 'E', []()
			{
				 if (Hexeng::scene_id != 2)
					 return;

				Player::get()->press();
				auto now = std::chrono::high_resolution_clock::now();
				Player::get()->registered_moves.push_back({ now - Player::get()->last_move, MoveDirection::INTERACT });
				Player::get()->last_move = now;
			}
		 };

		 ESCAPE = { GLFW_KEY_ESCAPE,[]()
			{
				 if (Hexeng::scene_id == 0)
					 Hexeng::exit = true;
				 if (Hexeng::scene_id == 1)
					 Hexeng::Scene::switch_scene(0);
				 if (Hexeng::scene_id == 2)
				 {
					 Hexeng::Scene::switch_scene(1);
					 gems_count = 0;
					 Hexeng::Renderer::pending_actions.push_back([]() {Player::storage.clear(); });
				 }
			}
		 };

		 R = { 'R', []()
			{
				 if (Hexeng::scene_id != 2)
					 return;

				 Hexeng::Renderer::pending_actions.push_back([]() {
					 Player::storage.clear();
					 gems_count = 0;
					 std::memcpy(map, current_selection_item->map, 1024);
					 build_board(map);
					 start = std::chrono::high_resolution_clock::now();
					 });
			}
		 };
	}
};

void Player::press()
{
	uint8_t facing_pos = pos;
	if (mesh.rotation == 0)
	{
		if (pos >= 240)
			return;
		facing_pos += 16;
	}
	else if (mesh.rotation == 90)
	{
		if (!(pos % 16 < 15))
			return;
		facing_pos++;
	}
	else if (mesh.rotation == -90)
	{
		if (!(pos % 16 > 0))
			return;
		facing_pos--;
	}
	else if (mesh.rotation == 180)
	{
		if (!(pos > 15))
			return;
		facing_pos -= 16;
	}
	behaviours[board[facing_pos].id].press(&board[facing_pos], this);
}

void Player::replay()
{
	for (auto& move_to_make : registered_moves)
	{
		std::unique_lock lk{ m };
		cv.wait_for(lk, move_to_make.wait_for, [this]() {return end_replay; });
		if (end_replay)
		{
			cv.notify_one();
			return;
		}
		switch (move_to_make.direction)
		{
			case MoveDirection::UP:		move(pos < 240, 16, { 0, 64 }, 0);		break;
			case MoveDirection::LEFT:	move(pos % 16 > 0, -1, { -64, 0 }, -90);break;
			case MoveDirection::DOWN:	move(pos > 15, -16, { 0, -64 }, 180);	break;
			case MoveDirection::RIGHT:	move(pos % 16 < 15, 1, { 64, 0 }, 90);	break;
			case MoveDirection::INTERACT: press(); break;
		}
	}
}

std::u32string to_u32string(int i)
{
	std::u32string output;
	std::string str = std::to_string(i);
	output.reserve(str.size());
	for (auto c : str)
		output.push_back(c);
	return output;
}

void update_gems_display()
{
	gem_display = Hexeng::Text{ to_u32string(gems_count), roboto,  {-710, 400}, 80, Hexeng::HorizontalAlign::LEFT };
}