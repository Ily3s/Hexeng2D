#include "Resources.hpp"
#include "Renderer/Renderer.hpp"

Hexeng::Renderer::Texture hexeng2d_logo;

Hexeng::Font roboto_font;

Hexeng::Renderer::Texture full_logo;

Hexeng::Language english{ "res/langs/English.txt" };
Hexeng::Language french{ "res/langs/French.txt" };

const Hexeng::Language* language_switch = &english;

Hexeng::Audio::Sound button_hover_snd;
Hexeng::Audio::Sound button_click_snd;

Hexeng::Renderer::Texture golden_frame_tex;

Hexeng::Renderer::ToBeInit init_resources
{ []()
	{

		golden_frame_tex = Hexeng::Renderer::Texture{"res/gold_frame.png"};

		hexeng2d_logo = { "res/hexeng2d_logo.png", {} };

		roboto_font = { "res/Roboto-Regular.ttf" };

		full_logo = { "res/full_logo.png" };

		Hexeng::Language::set_reference_language(&english);

		button_hover_snd = Hexeng::Audio::Sound{ "res/button_hover.wav" };
		button_click_snd = Hexeng::Audio::Sound{ "res/button_click.wav" };

	}, 1
};
