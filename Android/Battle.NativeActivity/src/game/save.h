#pragma once

#include "engine/data.h"

#include <stdbool.h>

typedef struct Save Save;

struct Save
{
	bool play_music;

	bool play_sounds;

	bool random_objects;

	bool same_random_car;

	int sudden_death_time;
};

extern Save g_save;

void save_save();

void save_load();
