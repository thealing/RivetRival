#include "button.h"

Button* button_create(const Shape* shape, char keybind, Texture* texture_pressed, Texture* texture_released, Vector position, double scale)
{
	Button* button = calloc(1, sizeof(Button));

	button->shape = shape_clone(shape);

	button->key = keybind;

	button->texture_pressed = texture_pressed;

	button->texture_released = texture_released;

	button->position = position;

	button->scale = scale;

	return button;
}

void button_destroy(Button* button)
{
	shape_destroy(button->shape);

	free(button);
}

void button_update(Button* button)
{
	if (window_is_mouse_button_pressed())
	{
		button->down = true;
	}

	if (!shape_test_point(button->shape, get_mouse_position()))
	{
		button->down = false;
	}

	if (window_is_mouse_button_released())
	{
		button->clicked = button->down;

		button->down = false;
	}
	else
	{
		button->clicked = false;
	}

	if (window_is_key_down(button->key))
	{
		button->down = true;
	}

	if (window_is_key_released(button->key))
	{
		button->clicked = true;

		button->down = false;
	}
}

void button_render(Button* button)
{
	Texture* texture = button->down ? button->texture_pressed : button->texture_released;

	set_texture_and_color(texture, NULL);

	graphics_store();

	graphics_translate(button->position);

	double size = fmax(texture->size.x, texture->size.y);

	graphics_scale(vector_divide(vector_create(button->scale, button->scale), size));

	graphics_draw_texture();

	graphics_restore();
}

bool button_was_clicked(Button* button)
{
	bool clicked = button->clicked;

	button->clicked = false;

	return clicked;
}
