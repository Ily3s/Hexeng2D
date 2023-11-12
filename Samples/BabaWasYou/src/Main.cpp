#include "Menu.hpp"

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Audio/Audio.hpp"
#include "EventManager/EventManager.hpp"
#include "Scene.hpp"
#include "Board.hpp"
#include "Player.hpp"

uint8_t* map = new uint8_t[1024];

int main()
{
	Hexeng::Settings::window_name = "Baba was you";
	Hexeng::Settings::fullscreen = false;
	Hexeng::Settings::window_size = { 1600, 900 };

	Hexeng::Audio::init();
	Hexeng::Renderer::init();

	Hexeng::EventManager::start_looping();

	Hexeng::game_loop();

	Hexeng::EventManager::stop_looping();

	scores_save.save("scores.hxg_save");

	Hexeng::Renderer::stop();
	Hexeng::Audio::terminate();

	delete[] map;

	return 0;
}