#include "checkbox.h"

Checkbox* checkbox_create(Vector position, double half_size, bool* value)
{
	Checkbox* checkbox = malloc(sizeof(Checkbox));

	checkbox->rect = (Rect){ vector_subtract_value(position, half_size), vector_add_value(position, half_size) };

	checkbox->value = value;

	return checkbox;
}

void checkbox_destroy(Checkbox* checkbox)
{
	free(checkbox);
}

void checkbox_update(Checkbox* checkbox)
{
	if (window_is_mouse_button_pressed() && test_point_rect(get_mouse_position(), &checkbox->rect))
	{
		*checkbox->value = !*checkbox->value;

		sound_play(g_sounds.click);
	}
}

void checkbox_render(Checkbox* checkbox)
{
	set_texture_and_color(*checkbox->value ? g_textures.ui_checked : g_textures.ui_unchecked, NULL);

	graphics_draw_texture_in_rect(&checkbox->rect);
}
