#pragma once

#include <stdbool.h>

extern bool g_debug_hud;

extern int g_sudden_death_countdown;

extern int g_max_score;

extern int g_selected_level;

extern int g_selected_blue_car;

extern int g_selected_red_car;

extern bool g_blue_is_bot;

extern bool g_red_is_bot;

void global_init();
