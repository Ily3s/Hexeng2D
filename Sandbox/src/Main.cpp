#include <thread>
#include <chrono>

#include "Hexeng.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Presets/BasicQuad.hpp"
#include "Renderer/Layer.hpp"
#include "Renderer/Scene.hpp"

int main()
{
	using namespace Hexeng;

	Renderer::init();

	Renderer::Texture example{ "res/example.png", GL_NEAREST };
	Renderer::Presets::BasicSquare square{ { 0, 0 }, 10.0f, &example, true };

	Renderer::Layer layer_1{ {&square} };
	Renderer::Scene scene_1{ { &layer_1 } };

	Hexeng::game_loop([]() {
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(16ms);
		});

	Renderer::stop();

	return 0;
}