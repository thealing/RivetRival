#include "main.h"

static double s_update_fps;

static double s_render_fps;

static bool s_hud;

int main()
{
	window_create(1280, 720, "Rivet Rival");
	
	init();

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

	while (window_is_open()) 
	{
		double current_time = get_time();
		
		if (current_time > last_update_time + update_delta)
		{
			window_update();
			
			update(update_delta);
			
			last_update_time = fmax(last_update_time + update_delta, current_time - lag);
			
			update_count++;
		}
		
		if (current_time > last_render_time + render_delta)
		{
			graphics_clear(&(Color){ 1.0, 0.0, 1.0, 1.0 });
			
			render();
			
			window_display();
			
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

	scene_change(NULL);

	window_destroy();

	return 0;
}

void init()
{
	assets_init();

	global_init();
	
	menu_init();

	battle_init();
	
	graphics_set_font(texture_create_from_file("images/font.png"));

	graphics_set_camera(&(Rect){ 0, 0, 1280, 720 });
	
	scene_change(&g_menu);
}

void update(double delta_time)
{
	scene_update(delta_time);

	if (window_is_key_pressed('B'))
	{
		s_hud = !s_hud;
	}
}

void render()
{
	scene_render();

	if (s_hud)
	{
		graphics_draw_format_in_rect(&(Rect){ 0, 0, 0, 30 }, ALIGNMENT_LEFT, "%5d %3d", lround(s_update_fps), lround(s_render_fps));
	}
}
