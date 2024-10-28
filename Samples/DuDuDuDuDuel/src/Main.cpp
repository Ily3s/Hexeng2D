//#define HXG_DEBUG_LEVEL 1

#include "Hexeng.hpp"
#include "Scene.hpp"
#include "Renderer/Renderer.hpp"
#include "Audio/Audio.hpp"
#include "EventManager/EventManager.hpp"

#include "Resources.hpp"

int main()
{
	Hexeng::Settings::window_name = "DuDuDuDuDuel";
	Hexeng::Settings::fullscreen = true;

	Hexeng::Audio::init();
	Hexeng::Renderer::init();

	Hexeng::EventManager::start_looping();

	Hexeng::game_loop();

	Hexeng::EventManager::stop_looping();

	Hexeng::Renderer::stop();
	Hexeng::Audio::terminate();

	return 0;
}
