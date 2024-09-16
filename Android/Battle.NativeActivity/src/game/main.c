#include "main.h"

static double s_update_fps;

static double s_render_fps;

void android_main()
{
	bool started = false;

	double width = 1;

	double height = 1;

	double lag = 0.1;

	double update_delta = 1.0 / 260.0;

	double render_delta = 1.0 / 60.0;

	double measure_delta = 1.0;

	double start_time = get_time();

	double last_update_time = start_time;

	double last_render_time = start_time;

	double last_measure_time = start_time;

	int update_count = 0;

	int render_count = 0;

	while (true) 
	{
		double current_time = get_time();

		if (current_time > last_update_time + update_delta)
		{
			memcpy(g_previous_touches, g_current_touches, sizeof(g_previous_touches));

			memcpy(g_previous_keys, g_current_keys, sizeof(g_previous_keys));

			Android_Event e;

			while (android_poll_event(&e))
			{
				switch (e.type)
				{
					case ANDROID_EVENT_RESUMED:
					{
						if (started)
						{
							scene_resume();
						}

						break;
					}
					case ANDROID_EVENT_PAUSED:
					{
						if (started)
						{
							scene_pause();
						}

						break;
					}
					case ANDROID_EVENT_WINDOW_CREATED:
					{
						width = window_get_width(e.window_event.window);

						height = window_get_height(e.window_event.window);

						graphics_init(e.window_event.window);

						textures_init();

						graphics_set_font(g_textures.font);

						graphics_set_camera(&(Rect){ 0, 0, 1280, 720 });

						if (!started)
						{
							sounds_init();

							global_init();

							menu_init();

							settings_init();

							battle_init();

							scene_change(&g_settings);

							started = true;
						}

						break;
					}
					case ANDROID_EVENT_WINDOW_DESTROYED:
					{
						graphics_uninit(e.window_event.window);

						break;
					}
					case ANDROID_EVENT_TOUCH_DOWN:
					case ANDROID_EVENT_TOUCH_MOVE:
					{
						g_current_touches[e.touch_event.index].down = true;

						g_current_touches[e.touch_event.index].x = e.touch_event.x / width;

						g_current_touches[e.touch_event.index].y = 1 - e.touch_event.y / height;

						break;
					}
					case ANDROID_EVENT_TOUCH_UP:
					{
						g_current_touches[e.touch_event.index].down = false;

						break;
					}
					case ANDROID_EVENT_KEY_DOWN:
					{
						g_current_keys[e.key_event.key] = true;

						break;
					}
					case ANDROID_EVENT_KEY_UP:
					{
						g_current_keys[e.key_event.key] = false;

						break;
					}
				}
			}

			update(update_delta);

			last_update_time = fmax(last_update_time + update_delta, current_time - lag);

			update_count++;
		}

		if (current_time > last_render_time + render_delta)
		{
			graphics_clear(&(Color){ 1.0, 0.0, 1.0, 1.0 });

			render();

			graphics_display();

			last_render_time = current_time;

			render_count++;
		}

		if (current_time > last_measure_time + measure_delta)
		{
			s_update_fps = update_count / (current_time - last_measure_time);

			s_render_fps = render_count / (current_time - last_measure_time);

			update_count = 0;

			render_count = 0;

			last_measure_time = current_time;
		}

		sleep(fmin(last_update_time + update_delta - current_time, last_render_time + render_delta - current_time));
	}
}

void update(double delta_time)
{
	scene_update(delta_time);
}

void render()
{
	scene_render();

	if (g_debug_hud)
	{
		graphics_draw_format_in_rect(&(Rect){ 0, 0, 0, 30 }, ALIGNMENT_LEFT, "%5d %3d", lround(s_update_fps), lround(s_render_fps));
	}
}
