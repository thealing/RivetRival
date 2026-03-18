#include "global.h"

int g_save.max_score;

int g_sudden_death_time;

int g_save.sudden_death_countdown;

int g_save.selected_level;

int g_save.selected_blue_car;

int g_save.selected_red_car;

bool g_save.blue_is_bot;

bool g_save.red_is_bot;

void global_init()
{
	g_save.max_score = 5;

	g_sudden_death_time = 12;

	g_save.sudden_death_countdown = 3;

	g_save.selected_level = 1;

	g_save.selected_blue_car = 1;

	g_save.selected_red_car = 1;

	g_save.blue_is_bot = false;

	g_save.red_is_bot = true;
}
