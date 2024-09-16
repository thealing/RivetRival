#pragma once

#include "assets.h"

#include "engine/util.h"

typedef struct Button Button;

struct Button
{
	Shape* shape;

	char key;

	Texture* texture_pressed;

	Texture* texture_released;

	Vector position;

	double scale;

	bool down;

	bool clicked;
};

Button* button_create(const Shape* shape, char keybind, Texture* texture_pressed, Texture* texture_released, Vector position, double scale);

void button_destroy(Button* button);

void button_update(Button* button);

void button_render(Button* button);

bool button_was_clicked(Button* button);

