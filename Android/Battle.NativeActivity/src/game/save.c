#include "save.h"

Save g_save;

void save_save()
{
	data_write("save", &g_save, sizeof(g_save));
}

void save_load()
{
	g_save.play_music = true;

	g_save.play_sounds = true;

	g_save.random_objects = true;

	g_save.same_random_car = false;

	g_save.sudden_death_time = 12;

	data_read("save", &g_save, sizeof(g_save));
}
