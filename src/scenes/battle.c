#include "battle.h"

static const int s_level_group = -1;

static const int s_blue_car_group = -2;

static const int s_red_car_group = -3;

static const double s_death_delay = 2;

static const double s_air_time_threshold = 0.1;

static bool s_debug;

static bool s_paused;

static bool s_over;

static bool s_sudden_death;

static bool s_draw_countdown;

static bool s_beep_countdown;

static double s_elapsed_time;

static double s_win_time;

static int s_winner;

static int s_blue_score;

static int s_red_score;

static bool s_blue_player;

static bool s_red_player;

static double s_blue_contact_time;

static double s_red_contact_time;

static Bot* s_blue_bot;

static Bot* s_red_bot;
	
static Physics_World* s_world;

static Level* s_level;

static Car* s_blue_car;

static Car* s_red_car;

static Button* s_blue_brake;

static Button* s_blue_gas;

static Button* s_red_brake;

static Button* s_red_gas;

static Button* s_pause;

static Button* s_resume;

static Button* s_back;

static Button* s_restart;

static Button* s_big_back;

static Button* s_big_restart;

static void on_start()
{
	s_sudden_death = false;

	s_elapsed_time = 0;

	s_win_time = 0;

	s_winner = 0;

	s_level = level_create(g_selected_level == 0 ? random_below(LEVEL_TYPE_COUNT) : g_selected_level - 1, s_world, s_level_group);

	s_blue_car = car_create(g_selected_blue_car == 0 ? random_below(CAR_TYPE_COUNT) : g_selected_blue_car - 1, s_world, vector_create(300, 200), s_blue_car_group);

	s_red_car = car_create(g_selected_red_car == 0 ? random_below(CAR_TYPE_COUNT) : g_selected_red_car - 1, s_world, vector_create(980, 200), s_red_car_group);

	s_blue_contact_time = 1;

	s_red_contact_time = 1;

	bot_reset(s_blue_bot);

	bot_reset(s_red_bot);

	if (s_blue_player)
	{
		s_blue_brake->down = false;

		s_blue_gas->down = false;

		scene_add_button(s_blue_brake);

		scene_add_button(s_blue_gas);
	}

	if (s_red_player)
	{
		s_red_brake->down = false;

		s_red_gas->down = false;

		scene_add_button(s_red_brake);

		scene_add_button(s_red_gas);
	}
}

static void on_death()
{
	s_win_time = 0;

	s_blue_car->done = true;

	s_red_car->done = true;

	level_stop_armageddon(s_level);

	sound_play(g_sounds.airhorn);

	if (s_blue_player)
	{
		scene_remove_button(s_blue_brake);

		scene_remove_button(s_blue_gas);
	}

	if (s_red_player)
	{
		scene_remove_button(s_red_brake);

		scene_remove_button(s_red_gas);
	}
}

static void on_reset()
{
	car_destroy(s_blue_car);

	car_destroy(s_red_car);

	level_destroy(s_level);
}

static bool on_collision(Physics_Collider* collider, Physics_Collider* other_collider)
{
	if (collider->filter_group == s_blue_car_group && other_collider->filter_group == s_level_group)
	{
		s_blue_contact_time = s_elapsed_time;
	}

	if (collider->filter_group == s_red_car_group && other_collider->filter_group == s_level_group)
	{
		s_red_contact_time = s_elapsed_time;
	}

	return true;
}

void battle_init()
{
	g_battle.enter = battle_enter;
	
	g_battle.leave = battle_leave;
	
	g_battle.update = battle_update;
	
	g_battle.render = battle_render;
}

void battle_enter()
{
	s_paused = false;

	s_over = false;
	
	s_elapsed_time = 0;

	s_blue_score = 0;

	s_red_score = 0;

	s_blue_player = !g_blue_is_bot;

	s_red_player = !g_red_is_bot;

	s_blue_bot = bot_create();

	s_red_bot = bot_create();

	s_world = physics_world_create();

	s_world->gravity.y = -750.0;

	s_world->collision_callback = on_collision;

	if (s_blue_player && s_red_player)
	{
		s_blue_brake = button_create(move_shape(create_rect_shape(vector_create(0, 0), vector_create(130, 720))), 'A', g_textures.ui_brake_pressed[0], g_textures.ui_brake_released[0], vector_create(60, 60), 80);

		s_blue_gas = button_create(move_shape(create_rect_shape(vector_create(130, 0), vector_create(260, 720))), 'D', g_textures.ui_gas_pressed[0], g_textures.ui_gas_released[0], vector_create(200, 60), 80);

		s_red_brake = button_create(move_shape(create_rect_shape(vector_create(1150, 0), vector_create(1280, 720))), KEY_RIGHT, g_textures.ui_brake_pressed[1], g_textures.ui_brake_released[1], vector_create(1220, 60), 80);

		s_red_gas = button_create(move_shape(create_rect_shape(vector_create(1020, 0), vector_create(1150, 720))), KEY_LEFT, g_textures.ui_gas_pressed[1], g_textures.ui_gas_released[1], vector_create(1080, 60), 80);
	}
	else if (s_blue_player)
	{
		s_blue_brake = button_create(move_shape(create_rect_shape(vector_create(0, 0), vector_create(200, 720))), 'A', g_textures.ui_brake_pressed[0], g_textures.ui_brake_released[0], vector_create(60, 60), 80);

		s_blue_gas = button_create(move_shape(create_rect_shape(vector_create(1080, 0), vector_create(1280, 720))), 'D', g_textures.ui_gas_pressed[0], g_textures.ui_gas_released[0], vector_create(1220, 60), 80);
	}
	else if (s_red_player)
	{
		s_red_brake = button_create(move_shape(create_rect_shape(vector_create(1080, 0), vector_create(1280, 720))), 'D', g_textures.ui_brake_pressed[1], g_textures.ui_brake_released[1], vector_create(1220, 60), 80);

		s_red_gas = button_create(move_shape(create_rect_shape(vector_create(0, 0), vector_create(200, 720))), 'A', g_textures.ui_gas_pressed[1], g_textures.ui_gas_released[1], vector_create(60, 60), 80);
	}

	s_pause = button_create(move_shape(shape_create_circle(vector_create(640, 50), 32)), KEY_SPACE, g_textures.ui_pause_pressed, g_textures.ui_pause_released, vector_create(640, 50), 60);

	s_resume = button_create(move_shape(shape_create_circle(vector_create(640, 50), 32)), KEY_SPACE, g_textures.ui_play_pressed, g_textures.ui_play_released, vector_create(640, 50), 60);

	s_back = button_create(move_shape(shape_create_circle(vector_create(540, 50), 32)), KEY_BACK, g_textures.ui_back_pressed, g_textures.ui_back_released, vector_create(540, 50), 60);

	s_restart = button_create(move_shape(shape_create_circle(vector_create(740, 50), 32)), KEY_ENTER, g_textures.ui_restart_pressed, g_textures.ui_restart_released, vector_create(740, 50), 60);

	s_big_back = button_create(move_shape(shape_create_circle(vector_create(60, 60), 50)), KEY_BACK, g_textures.ui_back_pressed, g_textures.ui_back_released, vector_create(60, 60), 100);

	s_big_restart = button_create(move_shape(shape_create_circle(vector_create(640, 60), 50)), KEY_ENTER, g_textures.ui_restart_pressed, g_textures.ui_restart_released, vector_create(640, 60), 100);

	scene_add_button(s_pause);

	sound_play(g_sounds.music);

	on_start();
}

void battle_leave()
{
	on_reset();

	bot_destroy(s_blue_bot);

	bot_destroy(s_red_bot);
	
	physics_world_destroy(s_world);

	if (s_blue_player && s_red_player)
	{
		button_destroy(s_blue_brake);

		button_destroy(s_blue_gas);

		button_destroy(s_red_brake);

		button_destroy(s_red_gas);
	}
	else if (s_blue_player)
	{
		button_destroy(s_blue_brake);

		button_destroy(s_blue_gas);
	}
	else if (s_red_player)
	{
		button_destroy(s_red_brake);

		button_destroy(s_red_gas);
	}

	button_destroy(s_pause);

	button_destroy(s_resume);

	button_destroy(s_back);

	button_destroy(s_restart);

	button_destroy(s_big_back);

	button_destroy(s_big_restart);

	sound_stop(g_sounds.airhorn);

	sound_stop(g_sounds.applause);
}

void battle_update(double delta_time)
{
	if (window_is_key_pressed('B'))
	{
		s_debug = !s_debug;
	}

	if (button_was_clicked(s_pause))
	{
		s_paused = true;

		scene_remove_button(s_pause);

		scene_add_button(s_resume);

		scene_add_button(s_back);

		scene_add_button(s_restart);

		sound_pause(g_sounds.music);

		sound_stop(g_sounds.laser);
	}

	if (button_was_clicked(s_resume))
	{
		s_paused = false;

		scene_remove_button(s_resume);

		scene_remove_button(s_back);

		scene_remove_button(s_restart);

		scene_add_button(s_pause);

		sound_resume(g_sounds.music);
	}

	if (button_was_clicked(s_back) || button_was_clicked(s_big_back))
	{
		scene_change(&g_menu);

		return;
	}

	if (button_was_clicked(s_restart) || button_was_clicked(s_big_restart))
	{
		scene_change(&g_battle);

		return;
	}

	if (!s_paused)
	{
		if (s_winner == 0)
		{
			if (s_blue_car->done && s_red_car->done)
			{
				s_winner = 1;
			}
			else if (s_red_car->done)
			{
				s_winner = 2;

				s_blue_score++;
			}
			else if (s_blue_car->done)
			{
				s_winner = 3;

				s_red_score++;
			}

			if (s_winner != 0)
			{
				on_death();
			}
		}
		
		if (s_winner != 0)
		{
			if (!s_over && s_win_time > s_death_delay)
			{
				if (s_blue_score == g_max_score || s_red_score == g_max_score)
				{
					s_over = true;

					scene_remove_button(s_pause);

					scene_add_button(s_big_back);

					scene_add_button(s_big_restart);

					sound_stop(g_sounds.music);

					sound_play(g_sounds.applause);
				}
				else
				{
					on_reset();

					on_start();
				}
			}

			s_win_time += delta_time;
		}

		if (!s_over && !sound_is_playing(g_sounds.music))
		{
			sound_play(g_sounds.music);
		}

		if (!s_over && s_winner == 0)
		{
			if (!s_sudden_death && s_elapsed_time > g_sudden_death_time)
			{
				s_sudden_death = true;

				level_start_armageddon(s_level);
			}

			if (fmod(s_elapsed_time, 1) < delta_time)
			{
				double countdown = ceil(g_sudden_death_time - s_elapsed_time);

				if (countdown > 0 && countdown <= g_sudden_death_countdown)
				{
					sound_play(g_sounds.blip);
				}
			}
		}

		level_update(s_level);

		if (s_blue_player && s_red_player)
		{
			car_update(s_blue_car, s_blue_gas->down, s_blue_brake->down);

			car_update(s_red_car, s_red_gas->down, s_red_brake->down);
		}
		else
		{
			double blue_location = s_blue_car->chassis_body->position.x;

			double blue_angle = s_blue_car->chassis_body->angle;

			Vector blue_velocity = s_blue_car->chassis_body->linear_velocity;

			double red_location = s_red_car->chassis_body->position.x;

			double red_angle = s_red_car->chassis_body->angle;

			Vector red_velocity = s_red_car->chassis_body->linear_velocity;

			if (s_blue_player)
			{
				car_update(s_blue_car, s_blue_gas->down, s_blue_brake->down);
			}
			else
			{
				bot_update(s_blue_bot, s_elapsed_time > s_blue_contact_time + s_air_time_threshold, blue_location, blue_angle, blue_velocity, red_location, s_elapsed_time);

				car_update(s_blue_car, s_blue_bot->forward, s_blue_bot->backward);
			}

			if (s_red_player)
			{
				car_update(s_red_car, s_red_gas->down, s_red_brake->down);
			}
			else
			{
				bot_update(s_red_bot, s_elapsed_time > s_red_contact_time + s_air_time_threshold, 1280 - red_location, -red_angle, vector_create(red_velocity.x * -1, red_velocity.y), 1280 - blue_location, s_elapsed_time);

				car_update(s_red_car, s_red_bot->forward, s_red_bot->backward);
			}
		}

		physics_world_step(s_world, s_winner != 0 ? delta_time / 2 : delta_time);

		s_elapsed_time += delta_time;
	}
}

void battle_render()
{
	graphics_clear(&(Color){ 1.0, 1.0, 1.0, 1.0 });

	level_render(s_level);

	car_render(s_blue_car);

	car_render(s_red_car);

	if (s_debug)
	{
		draw_physics_world(s_world);
	}

	for (int i = 0; i < g_max_score; i++)
	{
		if (s_blue_score > i)
		{
			set_texture_and_color(g_textures.ui_star_opaque, &(Color){ 0, 0, 1, 1 });
		}
		else
		{
			set_texture_and_color(g_textures.ui_star_translucent, NULL);
		}

		graphics_draw_texture_at(vector_create(40 + i * 60, 680), s_winner == 2 && s_blue_score == i + 1 ? fmax(s_death_delay - s_win_time, 0) * 2 * M_PI : 0);
	}

	for (int i = 0; i < g_max_score; i++)
	{
		if (s_red_score > i)
		{
			set_texture_and_color(g_textures.ui_star_opaque, &(Color){ 1, 0, 0, 1 });
		}
		else
		{
			set_texture_and_color(g_textures.ui_star_translucent, NULL);
		}

		graphics_draw_texture_at(vector_create(1240 - i * 60, 680), s_winner == 3 && s_red_score == i + 1 ? fmin(s_win_time, s_death_delay) * 2 * M_PI : 0);
	}

	if (s_over)
	{
		graphics_store();

		graphics_translate(vector_create(640, 360));
		
		graphics_scale(create_diagonal(100 * (1 + fabs(frac(s_win_time) - 0.5) * 0.3)));

		switch (s_winner)
		{
			case 2:
			{
				graphics_set_color(&(Color){ 0, 0, 1, 1 });

				graphics_draw_string(ALIGNMENT_CENTER, ALIGNMENT_CENTER, "BLUE WON!");

				break;
			}
			case 3:
			{
				graphics_set_color(&(Color){ 1, 0, 0, 1 });

				graphics_draw_string(ALIGNMENT_CENTER, ALIGNMENT_CENTER, "RED WON!");

				break;
			}
		}

		graphics_restore();
	}

	if (!s_over && s_winner != 0 && fmod(s_win_time, 0.4) < 0.2)
	{
		Rect rect = { 0, 550, 1280, 620 };

		switch (s_winner)
		{
			case 1:
			{
				graphics_set_color(&(Color){ 0, 1, 0, 1 });

				graphics_draw_string_in_rect(&rect, ALIGNMENT_CENTER, "TIE!");

				break;
			}
			case 2:
			{
				graphics_set_color(&(Color){ 0, 0, 1, 1 });

				graphics_draw_string_in_rect(&rect, ALIGNMENT_CENTER, "BLUE SCORES!");

				break;
			}
			case 3:
			{
				graphics_set_color(&(Color){ 1, 0, 0, 1 });

				graphics_draw_string_in_rect(&rect, ALIGNMENT_CENTER, "RED SCORES!");

				break;
			}
		}
	}

	if (!s_over && s_winner == 0)
	{
		if (fmod(s_elapsed_time, 1) < 0.7)
		{
			double countdown = ceil(g_sudden_death_time - s_elapsed_time);

			Rect rect = { 0, 580, 1280, 640 };

			graphics_set_color(&(Color){ 0.25, 0.75, 0.25, 1 });

			if (countdown == 0)
			{
				graphics_draw_string_in_rect(&rect, ALIGNMENT_CENTER, "SUDDEN DEATH!");
			}

			if (countdown > 0 && countdown <= g_sudden_death_countdown)
			{
				graphics_draw_format_in_rect(&rect, ALIGNMENT_CENTER, "%d", lround(countdown));
			}
		}
	}

	if (s_paused)
	{
		graphics_set_color(&(Color){ 0.25, 0.75, 0.25, 1 });

		graphics_draw_string_in_rect(&(Rect){ 0, 310, 1280, 410 }, ALIGNMENT_CENTER, "PAUSED");
	}

	if (s_debug)
	{
		graphics_set_color(&(Color){ 1, 1, 1, 1 });

		graphics_draw_format_in_rect(&(Rect){ 40, 540, 40, 580 }, ALIGNMENT_LEFT, "%.1f", s_elapsed_time - s_blue_contact_time);

		graphics_draw_format_in_rect(&(Rect){ 1240, 540, 1240, 580 }, ALIGNMENT_RIGHT, "%.1f", s_elapsed_time - s_red_contact_time);
	}
}
