#include "levels.h"

static const Vector s_cave_bottom[] = {{74,365},{80,309},{93,262},{110,224},{129,192},{149,167},{180,138},{213,112},{254,90},{290,78},{330,66},{927,66},{962,73},{1003,83},{1038,97},{1074,119},{1103,142},{1128,167},{1150,193},{1170,227},{1186,261},{1195,294},{1203,332},{1204,364}};

static const Vector s_desert_bottom[] = {{76,383},{75,347},{80,311},{88,280},{100,246},{113,218},{142,195},{173,175},{200,160},{224,143},{248,131},{274,119},{308,108},{349,105},{398,104},{443,112},{482,120},{520,133},{562,145},{608,151},{672,151},{723,144},{765,132},{810,118},{855,109},{889,104},{933,105},{978,111},{1013,122},{1051,142},{1086,164},{1110,180},{1133,193},{1152,209},{1169,222},{1182,251},{1192,283},{1202,321},{1205,353},{1205,375}};

static const Vector s_cave_top[] = {{74,364},{78,405},{87,444},{100,483},{118,517},{138,547},{164,576},{193,600},{220,618},{255,636},{290,649},{323,655},{356,658},{936,658},{982,652},{1017,639},{1043,628},{1073,610},{1099,590},{1123,567},{1145,543},{1161,516},{1175,488},{1187,461},{1196,425},{1203,388},{1204,364}};

static const Vector s_center[] = {{297,280},{977,280},{993,294},{992,432},{980,442},{297,442},{283,430},{282,295}};

static const Vector s_desert_on_center[] = {{311,443},{347,474},{383,490},{429,491},{473,477},{501,468},{539,466},{583,474},{613,485},{654,484},{685,477},{722,469},{758,467},{792,472},{821,483},{848,490},{886,491},{916,480},{942,462},{964,445}};

static const Vector s_ramp_bottom[] = {{76,378},{76,347},{80,313},{88,279},{102,242},{118,211},{142,178},{169,149},{195,126},{219,109},{247,95},{277,83},{311,73},{379,73},{414,77},{443,85},{468,91},{492,101},{517,116},{540,132},{565,151},{592,179},{610,205},{628,236},{637,261},{648,239},{666,207},{687,178},{710,153},{737,131},{774,107},{812,91},{851,79},{893,73},{966,74},{1001,83},{1038,97},{1076,120},{1107,144},{1135,173},{1158,207},{1178,243},{1192,279},{1200,318},{1203,369},{1203,380}};

static const Vector s_gap_bottom[] = {{75,380},{76,350},{80,320},{85,289},{93,264},{106,234},{122,206},{141,178},{165,152},{187,131},{224,107},{260,89},{299,77},{310,75},{354,75},{400,80},{435,91},{466,105},{500,124},{530,147},{530,29},{740,29},{740,150},{764,130},{786,115},{810,102},{840,90},{875,80},{910,74},{964,73},{1000,83},{1030,94},{1059,110},{1090,132},{1116,155},{1141,184},{1164,215},{1178,245},{1190,275},{1198,310},{1202,348},{1203,381}};

Level* level_create(Level_Type type, Physics_World* world, int group)
{
	Level* level = calloc(1, sizeof(Level));

	level->body = physics_body_create(world, PHYSICS_BODY_TYPE_STATIC);

	if (type == LEVEL_TYPE_CAVE || type == LEVEL_TYPE_TUNNEL)
	{
		for (int i = 0; i + 1 < countof(s_cave_bottom); i++)
		{
			Vector a = s_cave_bottom[i];

			Vector b = s_cave_bottom[i + 1];

			Vector c = vector_create(b.y > a.y ? b.x + 10 : b.x, 0);

			Vector d = vector_create(a.y > b.y ? a.x - 10 : a.x, 0);

			Shape* shape = shape_create_polygon(4, (Vector[]){ a, b, c, d });

			Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

			collider->static_friction = 0.8;

			collider->dynamic_friction = 0.8;

			collider->filter_group = group;

			shape_destroy(shape);
		}
	}

	if (type == LEVEL_TYPE_DESERT_CAVE || type == LEVEL_TYPE_DESERT_TUNNEL)
	{
		for (int i = 0; i + 1 < countof(s_desert_bottom); i++)
		{
			Vector a = s_desert_bottom[i];

			Vector b = s_desert_bottom[i + 1];

			Vector c = vector_create(b.y > a.y ? b.x + 10 : b.x, 0);

			Vector d = vector_create(a.y > b.y ? a.x - 10 : a.x, 0);

			Shape* shape = shape_create_polygon(4, (Vector[]){ a, b, c, d });

			Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

			collider->static_friction = 0.4;

			collider->dynamic_friction = 0.4;

			collider->filter_group = group;

			shape_destroy(shape);
		}
	}

	if (type == LEVEL_TYPE_TUNNEL || type == LEVEL_TYPE_DESERT_TUNNEL)
	{
		Shape* shape = shape_create_polygon(countof(s_center), s_center);

		Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

		collider->static_friction = 0.8;

		collider->dynamic_friction = 0.8;

		collider->filter_group = group;

		shape_destroy(shape);
	}

	if (type == LEVEL_TYPE_DESERT_TUNNEL)
	{
		for (int i = 0; i + 1 < countof(s_desert_on_center); i++)
		{
			Vector a = s_desert_on_center[i];

			Vector b = s_desert_on_center[i + 1];

			Vector c = vector_create(b.y > a.y ? b.x + 10 : b.x, 360);

			Vector d = vector_create(a.y > b.y ? a.x - 10 : a.x, 360);

			Shape* shape = shape_create_polygon(4, (Vector[]){ a, b, c, d });

			Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

			collider->static_friction = 0.4;

			collider->dynamic_friction = 0.4;

			collider->filter_group = group;

			shape_destroy(shape);
		}
	}

	if (type == LEVEL_TYPE_RAMP)
	{
		for (int i = 0; i + 1 < countof(s_ramp_bottom); i++)
		{
			Vector a = s_ramp_bottom[i];

			Vector b = s_ramp_bottom[i + 1];

			Vector c = vector_create(b.y > a.y ? b.x + 10 : b.x, 0);

			Vector d = vector_create(a.y > b.y ? a.x - 10 : a.x, 0);

			Shape* shape = shape_create_polygon(4, (Vector[]){ a, b, c, d });

			Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

			collider->static_friction = 0.8;

			collider->dynamic_friction = 0.8;

			collider->filter_group = group;

			shape_destroy(shape);
		}
	}

	if (type == LEVEL_TYPE_GAP)
	{
		for (int i = 0; i + 1 < countof(s_gap_bottom); i++)
		{
			Vector a = s_gap_bottom[i];

			Vector b = s_gap_bottom[i + 1];

			Vector c = vector_create(i != 18 && b.y > a.y ? b.x + 10 : b.x, 0);

			Vector d = vector_create(i != 22 && a.y > b.y ? a.x - 10 : a.x, 0);

			Shape* shape = shape_create_polygon(4, (Vector[]){ a, b, c, d });

			Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

			collider->static_friction = 0.8;

			collider->dynamic_friction = 0.8;

			collider->filter_group = group;

			shape_destroy(shape);
		}
	}

	if (true)
	{
		for (int i = 0; i + 1 < countof(s_cave_top); i++)
		{
			Vector a = s_cave_top[i];

			Vector b = s_cave_top[i + 1];

			Vector c = vector_create(b.y > a.y ? b.x + 10 : b.x, 720);

			Vector d = vector_create(a.y > b.y ? a.x - 10 : a.x, 720);

			Shape* shape = shape_create_polygon(4, (Vector[]){ a, b, c, d });

			Physics_Collider* collider = physics_collider_create(level->body, shape, 1);

			collider->static_friction = 0.8;

			collider->dynamic_friction = 0.8;

			collider->filter_group = group;

			shape_destroy(shape);
		}
	}

	switch (type)
	{
		case LEVEL_TYPE_CAVE:
		{
			level->texture = g_textures.level_cave;

			break;
		}
		case LEVEL_TYPE_TUNNEL:
		{
			level->texture = g_textures.level_tunnel;

			break;
		}
		case LEVEL_TYPE_DESERT_CAVE:
		{
			level->texture = g_textures.level_desert_cave;

			break;
		}
		case LEVEL_TYPE_DESERT_TUNNEL:
		{
			level->texture = g_textures.level_desert_tunnel;

			break;
		}
		case LEVEL_TYPE_RAMP:
		{
			level->texture = g_textures.level_ramp;

			break;
		}
		case LEVEL_TYPE_GAP:
		{
			level->texture = g_textures.level_gap;

			break;
		}
	}

	level->laser_direction = random_below(LASER_DIRECTION_COUNT);

	return level;
}

void level_destroy(Level* level)
{
	physics_body_destroy(level->body);

	if (level->laser_body != NULL)
	{
		physics_body_destroy(level->laser_body);
	}

	if (level->armageddon)
	{
		sound_stop(g_sounds.laser);
	}

	free(level);
}

void level_update(Level* level)
{
	if (level->armageddon)
	{
		if (!sound_is_playing(g_sounds.laser))
		{
			sound_play(g_sounds.laser);
		}
	}
}

void level_render(Level* level)
{
	set_texture_and_color(level->texture, NULL);

	graphics_draw_texture_in_rect(&(Rect){ 0, 0, 1280, 720 });

	if (level->laser_body != NULL)
	{
		set_texture_and_color(g_textures.laser, &(Color){ 0.2, 0.8, 1, random_fraction_in_range(0.9, 1) });

		graphics_draw_texture_at(level->laser_body->position, level->laser_body->angle);
	}
}

void level_start_armageddon(Level* level)
{
	level->armageddon = true;

	level->laser_body = physics_body_create(level->body->world, PHYSICS_BODY_TYPE_KINEMATIC);

	Physics_Collider* collider = physics_collider_create(level->laser_body, move_shape(shape_create_segment(vector_create(-1000, 0), vector_create(1000, 0))), 1);

	collider->sensor = true;

	switch (level->laser_direction)
	{
		case LASER_DIRECTION_UP:
		{
			level->laser_body->position = vector_create(640, 0);

			level->laser_body->linear_velocity = vector_create(0, 50);

			break;
		}
		case LASER_DIRECTION_DOWN:
		{
			level->laser_body->position = vector_create(640, 720);

			level->laser_body->linear_velocity = vector_create(0, -50);

			break;
		}
	}

	sound_play(g_sounds.laser);
}

void level_stop_armageddon(Level* level)
{
	level->armageddon = false;

	if (level->laser_body != NULL)
	{
		level->laser_body->linear_velocity = vector_create(0, 0);

		sound_stop(g_sounds.laser);
	}
}
