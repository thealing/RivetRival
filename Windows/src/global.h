#pragma once

#include <stdbool.h>

extern int g_save.max_score;

extern int g_sudden_death_time;

extern int g_save.sudden_death_countdown;

extern int g_save.selected_level;

extern int g_save.selected_blue_car;

extern int g_save.selected_red_car;

extern bool g_save.blue_is_bot;

extern bool g_save.red_is_bot;

void global_init();
