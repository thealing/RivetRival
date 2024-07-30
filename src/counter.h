#pragma once

#include "assets.h"

#include "engine/util.h"

typedef struct Counter Counter;

struct Counter
{
	Rect left_arrow;

	Rect right_arrow;

	Rect digits;

	int min;

	int max;

	int* value;
};

Counter* counter_create(Vector position, Vector size, int min, int max, int* value);

void counter_destroy(Counter* counter);

void counter_update(Counter* counter);

void counter_render(Counter* counter);
