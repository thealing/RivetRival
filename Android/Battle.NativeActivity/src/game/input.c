#include "input.h"

Touch g_current_touches[TOUCH_COUNT];

Touch g_previous_touches[TOUCH_COUNT];

bool g_current_keys[ANDROID_KEY_MAX];

bool g_previous_keys[ANDROID_KEY_MAX];

Vector input_project_touch(const Touch* touch)
{
	// TODO: unproject

	return vector_create(touch->x * 1280, touch->y * 720);
}

bool input_is_rect_down(const Rect* rect)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && test_point_rect(input_project_touch(&g_current_touches[i]), rect))
		{
			return true;
		}
	}

	return false;
}

bool input_is_segment_down(const Segment* segment)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && segment_test_point(segment, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_circle_down(const Circle* circle)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && circle_test_point(circle, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_polygon_down(const Polygon* polygon)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && polygon_test_point(polygon, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_shape_down(const Shape* shape)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && shape_test_point(shape, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_rect_pressed(const Rect* rect)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && !g_previous_touches[i].down && test_point_rect(input_project_touch(&g_current_touches[i]), rect))
		{
			return true;
		}
	}

	return false;
}

bool input_is_segment_pressed(const Segment* segment)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && !g_previous_touches[i].down && segment_test_point(segment, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_circle_pressed(const Circle* circle)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && !g_previous_touches[i].down && circle_test_point(circle, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_polygon_pressed(const Polygon* polygon)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && !g_previous_touches[i].down && polygon_test_point(polygon, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_shape_pressed(const Shape* shape)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (g_current_touches[i].down && !g_previous_touches[i].down && shape_test_point(shape, input_project_touch(&g_current_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_rect_released(const Rect* rect)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (!g_current_touches[i].down && g_previous_touches[i].down && test_point_rect(input_project_touch(&g_previous_touches[i]), rect))
		{
			return true;
		}
	}

	return false;
}

bool input_is_segment_released(const Segment* segment)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (!g_current_touches[i].down && g_previous_touches[i].down && segment_test_point(segment, input_project_touch(&g_previous_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_circle_released(const Circle* circle)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (!g_current_touches[i].down && g_previous_touches[i].down && circle_test_point(circle, input_project_touch(&g_previous_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_polygon_released(const Polygon* polygon)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (!g_current_touches[i].down && g_previous_touches[i].down && polygon_test_point(polygon, input_project_touch(&g_previous_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_shape_released(const Shape* shape)
{
	for (int i = 0; i < TOUCH_COUNT; i++)
	{
		if (!g_current_touches[i].down && g_previous_touches[i].down && shape_test_point(shape, input_project_touch(&g_previous_touches[i])))
		{
			return true;
		}
	}

	return false;
}

bool input_is_key_down(Android_Key key)
{
	return g_current_keys[key];
}

bool input_is_key_pressed(Android_Key key)
{
	return g_current_keys[key] && !g_previous_keys[key];
}

bool input_is_key_released(Android_Key key)
{
	return !g_current_keys[key] && g_previous_keys[key];
}
