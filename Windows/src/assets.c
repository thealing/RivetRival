#include "assets.h"

Textures g_textures;

Sounds g_sounds;

static void create_blue_red_textures(Texture* textures[2], const char* path)
{
	Image* image = image_load(path);

	textures[0] = texture_create_from_image(image);

	for (int i = 0; i < image->height; i++)
	{
		for (int j = 0, k = image->width - 1; j <= k; j++, k--)
		{
			char* left_pixel = (char*)image->pixels + (i * image->width + j) * 4;

			char* right_pixel = (char*)image->pixels + (i * image->width + k) * 4;

			char temp[4];

			memcpy(temp, left_pixel, 4);

			left_pixel[0] = right_pixel[2];

			left_pixel[1] = right_pixel[1];

			left_pixel[2] = right_pixel[0];

			left_pixel[3] = right_pixel[3];

			right_pixel[0] = temp[2];

			right_pixel[1] = temp[1];

			right_pixel[2] = temp[0];

			right_pixel[3] = temp[3];
		}
	}

	textures[1] = texture_create_from_image(image);

	image_destroy(image);
}

void assets_init()
{
	textures_init();

	sounds_init();
}

void textures_init()
{
	g_textures.laser = texture_create_from_file("images/laser.png");

	texture_resize(g_textures.laser, vector_create(1300, 4));

	g_textures.level_cave = texture_create_from_file("images/levels/cave.png");

	g_textures.level_tunnel = texture_create_from_file("images/levels/tunnel.png");

	g_textures.level_desert_cave = texture_create_from_file("images/levels/desert_cave.png");

	g_textures.level_desert_tunnel = texture_create_from_file("images/levels/desert_tunnel.png");

	g_textures.level_ramp = texture_create_from_file("images/levels/ramp.png");

	g_textures.level_gap = texture_create_from_file("images/levels/gap.png");

	g_textures.plank = texture_create_from_file("images/cars/plank.png");

	g_textures.garbage = texture_create_from_file("images/cars/garbage.png");

	create_blue_red_textures(g_textures.car_monster_truck, "images/cars/monster_truck.png");

	create_blue_red_textures(g_textures.car_dragster, "images/cars/dragster.png");

	create_blue_red_textures(g_textures.car_go_cart, "images/cars/go_cart.png");

	create_blue_red_textures(g_textures.car_minibus, "images/cars/minibus.png");

	create_blue_red_textures(g_textures.car_tractor, "images/cars/traktor.png");

	create_blue_red_textures(g_textures.car_timber_lorry, "images/cars/timber_lorry.png");

	create_blue_red_textures(g_textures.car_garbage_truck, "images/cars/garbage_truck.png");

	create_blue_red_textures(g_textures.wheel_tire, "images/wheels/tire.png");

	create_blue_red_textures(g_textures.wheel_small, "images/wheels/small.png");

	create_blue_red_textures(g_textures.wheel_cycle, "images/wheels/cycle.png");

	create_blue_red_textures(g_textures.wheel_monster, "images/wheels/monster.png");

	create_blue_red_textures(g_textures.head_neck, "images/head_neck.png");

	create_blue_red_textures(g_textures.head_only, "images/head_only.png");

	texture_resize(g_textures.head_neck[0], vector_create(32, 32));

	texture_resize(g_textures.head_neck[1], vector_create(32, 32));

	texture_resize(g_textures.head_only[0], vector_create(32, 32));

	texture_resize(g_textures.head_only[1], vector_create(32, 32));

	create_blue_red_textures(g_textures.timber_holder, "images/cars/timber_holder.png");

	create_blue_red_textures(g_textures.garbage_lid, "images/cars/garbage_lid.png");

	g_textures.overlay = texture_create_from_file("images/overlay.png");

	g_textures.ui_checked = texture_create_from_file("images/ui/checked.png");

	g_textures.ui_unchecked = texture_create_from_file("images/ui/unchecked.png");

	g_textures.ui_arrow_pressed = texture_create_from_file("images/ui/arrow_pressed.png");

	g_textures.ui_arrow_released = texture_create_from_file("images/ui/arrow_released.png");

	g_textures.ui_arrow_disabled = texture_create_from_file("images/ui/arrow_disabled.png");

	g_textures.ui_star_opaque = texture_create_from_file("images/ui/star_opaque.png");

	g_textures.ui_star_translucent = texture_create_from_file("images/ui/star_translucent.png");

	g_textures.ui_star_opaque->center.y = 56;

	g_textures.ui_star_translucent->center.y = 56;

	texture_resize(g_textures.ui_star_opaque, vector_create(50, 50));

	texture_resize(g_textures.ui_star_translucent, vector_create(50, 50));

	g_textures.ui_play_pressed = texture_create_from_file("images/ui/play_pressed.png");

	g_textures.ui_play_released = texture_create_from_file("images/ui/play_released.png");

	g_textures.ui_pause_pressed = texture_create_from_file("images/ui/pause_pressed.png");

	g_textures.ui_pause_released = texture_create_from_file("images/ui/pause_released.png");

	g_textures.ui_restart_pressed = texture_create_from_file("images/ui/restart_pressed.png");

	g_textures.ui_restart_released = texture_create_from_file("images/ui/restart_released.png");

	g_textures.ui_back_pressed = texture_create_from_file("images/ui/back_pressed.png");

	g_textures.ui_back_released = texture_create_from_file("images/ui/back_released.png");

	create_blue_red_textures(g_textures.ui_brake_pressed, "images/ui/left_pressed.png");

	create_blue_red_textures(g_textures.ui_brake_released, "images/ui/left_released.png");

	create_blue_red_textures(g_textures.ui_gas_pressed, "images/ui/right_pressed.png");

	create_blue_red_textures(g_textures.ui_gas_released, "images/ui/right_released.png");
}

void sounds_init()
{
	g_sounds.intro = sound_create("sounds/intro.mp2");

	g_sounds.music = sound_create("sounds/music.mp3");

	g_sounds.airhorn = sound_create("sounds/airhorn.mp3");

	g_sounds.applause = sound_create("sounds/applause.mp3");

	g_sounds.blip = sound_create("sounds/blip.mp3");

	g_sounds.click = sound_create("sounds/click.wav");

	g_sounds.click2 = sound_create("sounds/click2.wav");

	g_sounds.laser = sound_create("sounds/laser.mp2");

	sound_set_volume(g_sounds.intro, 0.6);

	sound_set_volume(g_sounds.laser, 0.4);
}
