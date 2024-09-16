#pragma once

#include "engine/geometry.h"

#include "engine/android.h"

#define TOUCH_COUNT 10

typedef struct Touch Touch;

struct Touch
{
	double x;

	double y;

	bool down;
};

extern Touch g_current_touches[TOUCH_COUNT];

extern Touch g_previous_touches[TOUCH_COUNT];

extern bool g_current_keys[ANDROID_KEY_MAX];

extern bool g_previous_keys[ANDROID_KEY_MAX];

Vector input_project_touch(const Touch* touch);

bool input_is_rect_down(const Rect* rect);

bool input_is_segment_down(const Segment* segment);

bool input_is_circle_down(const Circle* circle);

bool input_is_polygon_down(const Polygon* polygon);

bool input_is_shape_down(const Shape* shape);

bool input_is_rect_pressed(const Rect* rect);

bool input_is_segment_pressed(const Segment* segment);

bool input_is_circle_pressed(const Circle* circle);

bool input_is_polygon_pressed(const Polygon* polygon);

bool input_is_shape_pressed(const Shape* shape);

bool input_is_rect_released(const Rect* rect);

bool input_is_segment_released(const Segment* segment);

bool input_is_circle_released(const Circle* circle);

bool input_is_polygon_released(const Polygon* polygon);

bool input_is_shape_released(const Shape* shape);

bool input_is_key_down(Android_Key key);

bool input_is_key_pressed(Android_Key key);

bool input_is_key_released(Android_Key key);
