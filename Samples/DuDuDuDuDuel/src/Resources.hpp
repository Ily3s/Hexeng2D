#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "Renderer/Texture.hpp"
#include "Text.hpp"
#include "Audio/Audio.hpp"

extern Hexeng::Renderer::Texture hexeng2d_logo;

extern Hexeng::Font roboto_font;

extern Hexeng::Renderer::Texture full_logo;

extern Hexeng::Language english;
extern Hexeng::Language french;

extern const Hexeng::Language* language_switch;

extern Hexeng::Audio::Sound button_hover_snd;
extern Hexeng::Audio::Sound button_click_snd;

extern Hexeng::Renderer::Texture golden_frame_tex;

#endif