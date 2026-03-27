#include "window.h"

#include "platform.h"

int window_get_width(Window* window)
{
	return ANativeWindow_getWidth(window);
}

int window_get_height(Window* window)
{
	return ANativeWindow_getHeight(window);
}
