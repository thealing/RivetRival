#pragma once

#include "engine/graphics.h"

#include "engine/sound.h"

typedef struct Textures Textures;

typedef struct Sounds Sounds;

struct Textures
{
	Texture* laser;

	Texture* level_cave;

	Texture* level_tunnel;

	Texture* level_desert_cave;

	Texture* level_desert_tunnel;

	Texture* level_ramp;

	Texture* level_gap;

	Texture* plank;

	Texture* garbage;

	Texture* car_monster_truck[2];

	Texture* car_dragster[2];

	Texture* car_go_cart[2];

	Texture* car_minibus[2];

	Texture* car_tractor[2];

	Texture* car_timber_lorry[2];

	Texture* car_garbage_truck[2];

	Texture* wheel_tire[2];

	Texture* wheel_small[2];

	Texture* wheel_cycle[2];

	Texture* wheel_monster[2];

	Texture* head_neck[2];

	Texture* head_only[2];

	Texture* timber_holder[2];

	Texture* garbage_lid[2];

	Texture* overlay;

	Texture* ui_checked;

	Texture* ui_unchecked;

	Texture* ui_arrow_pressed;

	Texture* ui_arrow_released;

	Texture* ui_arrow_disabled;

	Texture* ui_star_opaque;

	Texture* ui_star_translucent;

	Texture* ui_play_pressed;

	Texture* ui_play_released;

	Texture* ui_pause_pressed;

	Texture* ui_pause_released;

	Texture* ui_restart_pressed;

	Texture* ui_restart_released;

	Texture* ui_back_pressed;

	Texture* ui_back_released;

	Texture* ui_brake_pressed[2];

	Texture* ui_brake_released[2];

	Texture* ui_gas_pressed[2];

	Texture* ui_gas_released[2];
};

struct Sounds
{
	Sound* intro;

	Sound* music;

	Sound* airhorn;

	Sound* applause;

	Sound* blip;

	Sound* click;

	Sound* click2;

	Sound* laser;
};

extern Textures g_textures;

extern Sounds g_sounds;

void assets_init();

void textures_init();

void sounds_init();
