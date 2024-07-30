#pragma once

#include "assets.h"

#include "engine/random.h"

#include "engine/util.h"

typedef enum Level_Type Level_Type;

typedef enum Laser_Direction Laser_Direction;

typedef struct Level Level;

enum Level_Type
{
	LEVEL_TYPE_CAVE,

	LEVEL_TYPE_TUNNEL,

	LEVEL_TYPE_DESERT_CAVE,

	LEVEL_TYPE_DESERT_TUNNEL,

	LEVEL_TYPE_RAMP,

	LEVEL_TYPE_GAP,

	LEVEL_TYPE_COUNT
};

enum Laser_Direction
{
	LASER_DIRECTION_UP,

	LASER_DIRECTION_DOWN,

	LASER_DIRECTION_COUNT
};

struct Level
{
	Physics_Body* body;

	Texture* texture;

	bool armageddon;

	Laser_Direction laser_direction;

	Physics_Body* laser_body;
};

Level* level_create(Level_Type type, Physics_World* world, int group);

void level_destroy(Level* level);

void level_update(Level* level);

void level_render(Level* level);

void level_start_armageddon(Level* level);

void level_stop_armageddon(Level* level);
