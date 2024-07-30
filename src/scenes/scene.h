#pragma once

#include "button.h"

#include "checkbox.h"

#include "counter.h"

#include <stddef.h>

#define BUTTON_COUNT_MAX 10

#define CHECKBOX_COUNT_MAX 10

#define COUNTER_COUNT_MAX 10

typedef struct Scene Scene;

struct Scene
{
	void (* enter)();
	
	void (* leave)();
	
	void (* update)(double delta_time);
	
	void (* render)();
};

extern Scene g_menu;

extern Scene g_battle;

void scene_change(Scene* scene);

void scene_update(double delta_time);

void scene_render();

void scene_add_button(Button* button);

void scene_remove_button(Button* button);

void scene_add_checkbox(Checkbox* checkbox);

void scene_remove_checkbox(Checkbox* checkbox);

void scene_add_counter(Counter* counter);

void scene_remove_counter(Counter* counter);
