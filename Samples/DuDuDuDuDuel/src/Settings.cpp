#include "Settings.hpp"
#include "Menu.hpp"
#include "Utils.hpp"

int scene_calling_settings = 0;

static DefaultButton return_btn;
Hexeng::EventManager::KeyEvent press_escape;
Hexeng::Text settings_txt;
Hexeng::Renderer::Quad separation_line_0;

Hexeng::Text language_text;
Hexeng::Renderer::Quad separation_line_1;
DefaultButton french_btn, english_btn;

Hexeng::Text sound_settings_text;
Hexeng::Renderer::Quad separation_line_2;
Cursor master_cursor;
Cursor sound_cursor;
Cursor music_cursor;

Hexeng::Renderer::Layer settings_layer{
	{&background, &return_btn.mesh, &settings_txt, &separation_line_0,
	&language_text, &separation_line_1, &french_btn.mesh, &english_btn.mesh,
	&sound_settings_text, &separation_line_2, &master_cursor.mesh, &sound_cursor.mesh, &music_cursor.mesh},
	0, Hexeng::Renderer::Position::ABSOLUTE };

Hexeng::Scene settings_scene{ 1, {
	{Hexeng::SceneComponent::LAYERS, {&settings_layer}},
	{Hexeng::SceneComponent::EVENTS, {
		&return_btn.button, &press_escape,
		&french_btn.button, &english_btn.button,
		&master_cursor.evt, &sound_cursor.evt, &music_cursor.evt}}
} };

int language_id = 0;

std::unordered_map<int, Hexeng::Language*> language_map
{
	{0, &english},
	{1, &french}
};

Hexeng::SaveFile settings_save;

Hexeng::Renderer::ToBeInit init_settings_save
{
	[]()
	{
		Hexeng::SaveVar language_id_save{0, &language_id, 4, 4};

		Hexeng::SaveVar master_volume_save{ 1, &Hexeng::Settings::master_volume, 4, 4 };
		Hexeng::SaveVar sound_volume_save{ 2, &Hexeng::Settings::sound_volume, 4, 4 };
		Hexeng::SaveVar music_volume_save{ 3, &Hexeng::Settings::music_volume, 4, 4 };

		settings_save.add_var(language_id_save);
		settings_save.add_var(master_volume_save);
		settings_save.add_var(sound_volume_save);
		settings_save.add_var(music_volume_save);

		try { settings_save.load("saves/settings.hxg_save"); }
		catch (const std::exception& e) {}

	}, 0
};

Hexeng::Renderer::ToBeInit init_settings_scene
{
	[]()
	{
		language_switch = language_map[language_id]; Hexeng::Text::reload_language();

		press_escape = Hexeng::EventManager::KeyEvent{GLFW_KEY_ESCAPE,
			[]() { Hexeng::Scene::switch_scene(scene_calling_settings); },
			Hexeng::Range::LOCAL, 10};
		return_btn = DefaultButton{ U"Return", []() { Hexeng::Scene::switch_scene(scene_calling_settings); },
			{-600, 470}, 10.0f, background.color };
		settings_txt = Hexeng::Text{ &language_switch, U"Settings", roboto_font, {0 , 400}, 125 };
		separation_line_0 = Hexeng::Renderer::Quad{ {0 , 375}, {99999, 4}, nullptr };
		separation_line_0.color = Hexeng::Color4::white;

		language_text = Hexeng::Text{ &language_switch, U"Language Selection", roboto_font, {0 , 250}, 75 };
		separation_line_1 = Hexeng::Renderer::Quad{ {0 , 225}, {99999, 2}, nullptr };
		separation_line_1.color = Hexeng::Color4::white;
		french_btn = DefaultButton{ U"French", []() { language_id = 1; language_switch = &french; Hexeng::Text::reload_language(); },
			{-350, 150}, 10.0f, background.color };
		english_btn = DefaultButton{ U"English", []() { language_id = 0; language_switch = &english; Hexeng::Text::reload_language(); },
			{350, 150}, 10.0f, background.color };

		sound_settings_text = Hexeng::Text{ &language_switch, U"Volume Settings", roboto_font, {0 , 0}, 75 };
		separation_line_2 = Hexeng::Renderer::Quad{ {0 , -25}, {99999, 2}, nullptr };
		separation_line_2.color = Hexeng::Color4::white;

		master_cursor = Cursor{ U"MASTER", &Hexeng::Settings::master_volume, {0, -150}, {900, 100} };
		sound_cursor = Cursor{ U"SOUND", &Hexeng::Settings::sound_volume, {0, -300}, {900, 100} };
		music_cursor = Cursor{ U"MUSIC", &Hexeng::Settings::music_volume, {0, -450}, {900, 100} };

	}, 2
};