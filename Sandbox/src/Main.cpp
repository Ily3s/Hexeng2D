#include <thread>
#include <chrono>

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Presets/BasicQuad.hpp"
#include "Renderer/Layer.hpp"
#include "Renderer/Scene.hpp"
#include "Renderer/Camera.hpp"

int main()
{
	using namespace Hexeng;

	Renderer::init();

	Renderer::Texture example{ "res/example.png", GL_NEAREST };
	Renderer::Presets::BasicSquare square{ { 0, 0 }, 5.0f, &example, true };
	Renderer::Presets::BasicSquare square2{ { -100, -100 }, 35.0f, &example, true };

	Renderer::Layer fore_ground{ {&square}, 500 };
	Renderer::Layer back_ground{ {&square2}, 1000 };
	Renderer::Scene first_scene{ 1, { &fore_ground , &back_ground } };

	Renderer::scene_id = 1;

	Hexeng::game_loop([]() {
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(16ms);
		});

	Renderer::stop();

	return 0;
}