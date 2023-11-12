#include "Intro.hpp"

Hexeng::Renderer::Quad hxg_logo_quad;

Hexeng::Text intro_text;

Hexeng::Renderer::Layer intro_layer;

Hexeng::Renderer::Animation intro_animation{ {
		{[](float t) {hxg_logo_quad.opacity += t / 1000;  intro_text.opacity += t / 1000; }, []() {}, 1000},
		{[](float t) {}, []() {}, 2000},
		{[](float t) {hxg_logo_quad.opacity -= t / 1000; intro_text.opacity -= t / 1000; }, []() {}, 1000},},
		[] {Hexeng::Scene::switch_scene(0); } };

Hexeng::Scene intro_scene{ -1, {{Hexeng::SceneComponent::LAYERS, {&intro_layer}}} };

Hexeng::Renderer::ToBeInit init_intro
{ []()
	{

	hxg_logo_quad = { {-380, 0}, 1.0f, &hexeng2d_logo };

	intro_text = { U"A game made possible by\nHexeng2D", roboto_font, {190, -30}, 70 };

	intro_layer = { {&hxg_logo_quad, &intro_text}, 0, Hexeng::Renderer::Position::ABSOLUTE };

	Hexeng::scene_id = -1;
	hxg_logo_quad.opacity = 0.0f;
	intro_text.opacity = 0.0f;
	intro_animation.play();

	}, 2
};