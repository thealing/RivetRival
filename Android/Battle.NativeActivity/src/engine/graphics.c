#include "graphics.h"

#include "platform.h"

static EGLDisplay display;

static EGLSurface surface;

static EGLDisplay context;

static State current_state;

static State state_history[GRAPHICS_HISTORY_SIZE];

static int state_history_index;

void texture_create_from_image(Texture** texture_pointer, const Image* image)
{
	if (*texture_pointer == NULL)
	{
		*texture_pointer = malloc(sizeof(Texture));
	}

	(*texture_pointer)->size = vector_create(image->width, image->height);

	(*texture_pointer)->center = vector_create(image->width / 2.0, image->height / 2.0);

	glGenTextures(1, &(*texture_pointer)->id);

	glBindTexture(GL_TEXTURE_2D, (*texture_pointer)->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_create_from_file(Texture** texture_pointer, const char* path)
{
	Image* image = image_load(path);

	texture_create_from_image(texture_pointer, image);

	image_destroy(image);
}

void texture_destroy(Texture* texture)
{
	glDeleteTextures(1, &texture->id);

	free(texture);
}

void texture_scale(Texture* texture, Vector scale)
{
	texture->size = vector_scale(texture->size, scale);

	texture->center = vector_scale(texture->center, scale);
}

void texture_resize(Texture* texture, Vector size)
{
	texture->center = vector_scale(texture->center, vector_downscale(size, texture->size));

	texture->size = size;
}

void texture_align(Texture* texture, Alignment horizontal_alignment, Alignment vertical_alignment)
{
	switch (horizontal_alignment)
	{
		case ALIGNMENT_LEFT:
		{
			texture->center.x = 0;

			break;
		}
		case ALIGNMENT_RIGHT:
		{
			texture->center.x = texture->size.x;

			break;
		}
		case ALIGNMENT_CENTER:
		{
			texture->center.x = texture->size.x / 2;

			break;
		}
	}

	switch (vertical_alignment)
	{
		case ALIGNMENT_BOTTOM:
		{
			texture->center.y = 0;

			break;
		}
		case ALIGNMENT_TOP:
		{
			texture->center.y = texture->size.y;

			break;
		}
		case ALIGNMENT_CENTER:
		{
			texture->center.y = texture->size.y / 2;

			break;
		}
	}
}

void graphics_init(void* window)
{
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	int egl_version_major;

	int egl_version_minor;

	eglInitialize(display, &egl_version_major, &egl_version_minor);

	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE,
	};

	EGLConfig config;

	int num_config;

	eglChooseConfig(display, attribs, &config, 1, &num_config);

	surface = eglCreateWindowSurface(display, config, window, NULL);

	context = eglCreateContext(display, config, NULL, NULL);

	eglMakeCurrent(display, surface, surface, context);

	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void graphics_uninit(void* window)
{
	eglDestroyContext(display, context);

	eglDestroySurface(display, surface);

	eglTerminate(display);

	display = EGL_NO_DISPLAY;

	context = EGL_NO_CONTEXT;

	surface = EGL_NO_SURFACE;
}

void graphics_display()
{
	eglSwapBuffers(display, surface);
}

void graphics_clear(const Color* color)
{
	glClearColor(color->red, color->green, color->blue, color->alpha);

	glClear(GL_COLOR_BUFFER_BIT);
}

void graphics_set_camera(const Rect* rect)
{
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrthof(rect->min.x, rect->max.x, rect->min.y, rect->max.y, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
}

void graphics_set_color(const Color* color)
{
	current_state.color = *color;

	glColor4f(color->red, color->green, color->blue, color->alpha);
}

void graphics_set_texture(const Texture* texture)
{
	if (current_state.texture == texture) 
	{
		return;
	}

	current_state.texture = texture;

	if (texture != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}
	else 
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void graphics_set_font(const Texture* font)
{
	current_state.font = font;
}

void graphics_set_line_width(double line_width)
{
	current_state.line_width = line_width;
}

void graphics_set_line_cap(Line_Cap line_cap)
{
	current_state.line_cap = line_cap;
}

void graphics_save_state()
{
	state_history[state_history_index] = current_state;

	state_history_index++;
}

void graphics_load_state()
{
	state_history_index--;

	State state = state_history[state_history_index];

	graphics_set_color(&state.color);

	graphics_set_texture(state.texture);

	graphics_set_font(state.font);

	graphics_set_line_width(state.line_width);

	graphics_set_line_cap(state.line_cap);
}

void graphics_save_transform()
{
	glPushMatrix();
}

void graphics_load_transform()
{
	glPopMatrix();
}

void graphics_translate(Vector translation)
{
	glTranslatef(translation.x, translation.y, 0.0);
}

void graphics_rotate(double angle)
{
	glRotatef(angle * 180.0 / M_PI, 0.0, 0.0, 1.0);
}

void graphics_scale(Vector factor)
{
	glScalef(factor.x, factor.y, 0.0);
}

void graphics_scale_uniformly(double factor)
{
	glScalef(factor, factor, 1.0);
}

void graphics_draw_segment(const Segment* segment, bool fill)
{
	GLfloat vertices[2][2] = { 
		{ segment->a.x, segment->a.y },
		{ segment->b.x, segment->b.y },
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, 2);
}

void graphics_draw_circle(const Circle* circle, bool fill)
{
	GLfloat vertices[GRAPHICS_CIRCLE_RESOLUTION + 2][2];

	vertices[0][0] = circle->center.x;

	vertices[0][1] = circle->center.y;

	for (int i = 0; i <= GRAPHICS_CIRCLE_RESOLUTION; i++) 
	{
		double angle = 2.0 * M_PI * i / GRAPHICS_CIRCLE_RESOLUTION;

		vertices[i + 1][0] = circle->center.x + circle->radius * cos(angle);

		vertices[i + 1][1] = circle->center.y + circle->radius * sin(angle);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glDrawArrays(fill ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, GRAPHICS_CIRCLE_RESOLUTION + 2);
}

void graphics_draw_polygon(const Polygon* polygon, bool fill)
{
	GLfloat vertices[polygon->point_count][2];

	for (int i = 0; i < polygon->point_count; i++) 
	{
		vertices[i][0] = polygon->points[i].x;

		vertices[i][1] = polygon->points[i].y;
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glDrawArrays(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, polygon->point_count);
}

void graphics_draw_shape(const Shape* shape, bool fill)
{
	switch (shape->type) 
	{
		case SHAPE_TYPE_SEGMENT: 
		{
			return graphics_draw_segment(&shape->segment, fill);
		}
		case SHAPE_TYPE_CIRCLE: 
		{
			return graphics_draw_circle(&shape->circle, fill);
		}
		case SHAPE_TYPE_POLYGON: 
		{
			return graphics_draw_polygon(&shape->polygon, fill);
		}
	}
}

void graphics_draw_rect(const Rect* rect, bool fill)
{
	GLfloat vertices[4][2] = {
		{ rect->min.x, rect->min.y },
		{ rect->max.x, rect->min.y },
		{ rect->max.x, rect->max.y },
		{ rect->min.x, rect->max.y },
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glDrawArrays(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
}

void graphics_draw_quad(const Vector quad[4], bool fill)
{
	GLfloat vertices[4][2] = {
		{ quad[0].x, quad[0].y },
		{ quad[1].x, quad[1].y },
		{ quad[2].x, quad[2].y },
		{ quad[3].x, quad[3].y },
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glDrawArrays(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
}

void graphics_draw_texture()
{
	if (current_state.texture == NULL)
	{
		return;
	}

	Vector center = current_state.texture->center;

	Vector size = current_state.texture->size;

	GLfloat vertices[] = {
		-center.x, -center.y,
		-center.x + size.x, -center.y,
		-center.x + size.x, -center.y + size.y,
		-center.x, -center.y + size.y
	};

	GLfloat texture_coords[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void graphics_draw_texture_at(Vector position, double rotation)
{
	if (current_state.texture == NULL)
	{
		return;
	}

	graphics_save_transform();

	graphics_translate(position);

	graphics_rotate(rotation);

	graphics_draw_texture();

	graphics_load_transform();
}

void graphics_draw_texture_in_rect(const Rect* rect)
{
	if (current_state.texture == NULL)
	{
		return;
	}

	GLfloat vertices[] = {
		rect->min.x, rect->min.y,
		rect->max.x, rect->min.y,
		rect->max.x, rect->max.y,
		rect->min.x, rect->max.y,
	};

	GLfloat texture_coords[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void graphics_draw_texture_in_quad(const Vector quad[4])
{
	if (current_state.texture == NULL)
	{
		return;
	}

	GLfloat vertices[] = {
		quad[0].x, quad[0].y,
		quad[1].x, quad[1].y,
		quad[2].x, quad[2].y,
		quad[3].x, quad[3].y,
	};

	GLfloat texture_coords[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void graphics_draw_texture_rect_in_rect(const Rect* texture_rect, const Rect* rect)
{
	if (current_state.texture == NULL)
	{
		return;
	}

	GLfloat vertices[] = {
		rect->min.x, rect->min.y,
		rect->max.x, rect->min.y,
		rect->max.x, rect->max.y,
		rect->min.x, rect->max.y,
	};

	GLfloat texture_coords[] = {
		texture_rect->min.x, texture_rect->min.y,
		texture_rect->max.x, texture_rect->min.y,
		texture_rect->max.x, texture_rect->max.y,
		texture_rect->min.x, texture_rect->max.y,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void graphics_draw_texture_rect_in_quad(const Rect* texture_rect, const Vector quad[4])
{
	if (current_state.texture == NULL)
	{
		return;
	}

	GLfloat vertices[] = {
		quad[0].x, quad[0].y,
		quad[1].x, quad[1].y,
		quad[2].x, quad[2].y,
		quad[3].x, quad[3].y,
	};

	GLfloat texture_coords[] = {
		texture_rect->min.x, texture_rect->min.y,
		texture_rect->max.x, texture_rect->min.y,
		texture_rect->max.x, texture_rect->max.y,
		texture_rect->min.x, texture_rect->max.y,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void graphics_draw_texture_quad_in_quad(const Vector texture_quad[4], const Vector quad[4])
{
	if (current_state.texture == NULL)
	{
		return;
	}

	GLfloat vertices[] = {
		quad[0].x, quad[0].y,
		quad[1].x, quad[1].y,
		quad[2].x, quad[2].y,
		quad[3].x, quad[3].y,
	};

	GLfloat texture_coords[] = {
		texture_quad[0].x, texture_quad[0].y,
		texture_quad[1].x, texture_quad[1].y,
		texture_quad[2].x, texture_quad[2].y,
		texture_quad[3].x, texture_quad[3].y,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void graphics_draw_character_in_rect(const Rect* rect, char character)
{
	if (current_state.font == NULL)
	{
		return;
	}

	// TODO: draw font without replacing the current texture

	graphics_set_texture(current_state.font);

	graphics_draw_texture_rect_in_rect(&(Rect){ character / 128.0, 0.0, (character + 1) / 128.0, 1.0 }, rect);
}

void graphics_draw_string_in_rect(const Rect* rect, Alignment alignment, const char* string)
{
	if (current_state.font == NULL)
	{
		return;
	}

	int string_length = strlen(string);

	double character_width = (rect->max.y - rect->min.y) / current_state.font->size.y * (current_state.font->size.x / 128.0);

	switch (alignment)
	{
		case ALIGNMENT_LEFT:
		{
			for (int i = 0; i < string_length; i++)
			{
				graphics_draw_character_in_rect(&(Rect){ { rect->min.x + character_width * i, rect->min.y }, { rect->min.x + character_width * (i + 1), rect->max.y } }, string[i]);
			}

			break;
		}
		case ALIGNMENT_RIGHT:
		{
			for (int i = 0; i < string_length; i++)
			{
				graphics_draw_character_in_rect(&(Rect){ { rect->max.x - character_width * (string_length - i), rect->min.y }, { rect->max.x - character_width * (string_length - i - 1), rect->max.y } }, string[i]);
			}

			break;
		}
		case ALIGNMENT_CENTER:
		{
			double start_x = (rect->min.x + rect->max.x) / 2.0 - character_width * string_length / 2.0;

			for (int i = 0; i < string_length; i++)
			{
				graphics_draw_character_in_rect(&(Rect){ { start_x + character_width * i, rect->min.y }, { start_x + character_width * (i + 1), rect->max.y } }, string[i]);
			}

			break;
		}
	}
}

void graphics_draw_format_in_rect(const Rect* rect, Alignment alignment, const char* format, ...)
{
	if (current_state.font == NULL)
	{
		return;
	}

	char string[120];

	va_list args;

	va_start(args, format);

	vsprintf(string, format, args);

	va_end(args);

	graphics_draw_string_in_rect(rect, alignment, string);
}

void graphics_draw_string(Alignment horizontal_alignment, Alignment vertical_alignment, const char* string)
{
	if (current_state.font == NULL)
	{
		return;
	}

	int string_length = strlen(string);

	double character_width = (current_state.font->size.x / 128.0) / current_state.font->size.y;

	double start_x = 0.0;

	double lower_y = 0.0;

	double upper_y = 0.0;

	switch (horizontal_alignment)
	{
		case ALIGNMENT_LEFT:
		{
			start_x = 0.0;

			break;
		}
		case ALIGNMENT_RIGHT:
		{
			start_x = character_width * -string_length;

			break;
		}
		case ALIGNMENT_CENTER:
		{
			start_x = character_width * -string_length / 2.0;

			break;
		}
	}

	switch (vertical_alignment)
	{
		case ALIGNMENT_BOTTOM:
		{
			lower_y = 0.0;

			upper_y = 1.0;

			break;
		}
		case ALIGNMENT_TOP:
		{
			lower_y = -1.0;

			upper_y = 0.0;

			break;
		}
		case ALIGNMENT_CENTER:
		{
			lower_y = -0.5;

			upper_y = 0.5;

			break;
		}
	}

	for (int i = 0; i < string_length; i++)
	{
		graphics_draw_character_in_rect(&(Rect){ { start_x + character_width * i, lower_y }, { start_x + character_width * (i + 1), upper_y } }, string[i]);
	}
}

void graphics_draw_format(Alignment horizontal_alignment, Alignment vertical_alignment, const char* format, ...)
{
	if (current_state.font == NULL)
	{
		return;
	}

	char string[120];

	va_list args;

	va_start(args, format);

	vsprintf(string, format, args);

	va_end(args);

	graphics_draw_string(horizontal_alignment, vertical_alignment, string);
}

void graphics_draw_line(Vector start, Vector end)
{
	double radius = current_state.line_width;

	Vector direction = vector_multiply(vector_normalize(vector_subtract(end, start)), radius);

	Vector normal = vector_left(direction);

	if (current_state.line_cap == LINE_CAP_SQUARE)
	{
		start = vector_subtract(start, direction);

		end = vector_add(end, direction);
	}

	Vector a = vector_subtract(start, normal);

	Vector b = vector_subtract(end, normal);

	Vector c = vector_add(end, normal);

	Vector d = vector_add(start, normal);

	graphics_draw_quad((Vector[4]){ a, b, c, d }, true);

	if (current_state.line_cap == LINE_CAP_ROUND)
	{
		graphics_draw_circle(&(Circle){ start, radius }, true);

		graphics_draw_circle(&(Circle){ end, radius }, true);
	}
}
