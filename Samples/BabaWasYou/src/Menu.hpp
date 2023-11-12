#ifndef MENU_HPP
#define MENU_HPP

#include "Hexeng.hpp"
#include "Scene.hpp"
#include "Renderer/Quad.hpp"
#include "EventManager/Button.hpp"
#include "Text.hpp"
#include "SaveSystem.hpp"

extern Hexeng::Scene menu_scene;
extern Hexeng::Scene map_selection;

struct SelectionItem
{
	Hexeng::Renderer::Quad icon;
	Hexeng::Renderer::Texture icon_texture;
	Hexeng::Renderer::Quad button_mesh;
	Hexeng::Renderer::Quad pass_mesh;
	Hexeng::EventManager::Button button;
	Hexeng::Text map_name;
	Hexeng::Text score_text;
	uint8_t* map;
	int index_in_save = -1;

	~SelectionItem();

	SelectionItem() = default;

	SelectionItem(SelectionItem&&) noexcept = default;
	SelectionItem& operator=(SelectionItem&&) noexcept = default;
};

extern std::vector<SelectionItem> map_items;

extern std::vector<uint8_t> scores_maps;
extern std::vector<uint64_t> actual_scores;

extern SelectionItem* current_selection_item;

extern Hexeng::SaveFile scores_save;

void scroll(double amount);

extern Hexeng::Renderer::Layer map_selection_layer;
extern Hexeng::Renderer::Layer scroll_bar_layer;

#endif // !MENU_HPP