#pragma once

#include "assets.h"

#include "engine/util.h"

typedef struct Checkbox Checkbox;

struct Checkbox
{
	Rect rect;
	
	bool* value;
};

Checkbox* checkbox_create(Vector position, double half_size, bool* value);

void checkbox_destroy(Checkbox* checkbox);

void checkbox_update(Checkbox* checkbox);

void checkbox_render(Checkbox* checkbox);
