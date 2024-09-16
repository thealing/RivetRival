#include "android.h"

#include "platform.h"

static bool started;

static pthread_t thread;

static pthread_mutex_t mutex;

static AInputQueue* input_queue;

static Android_Event_Node* first;

static Android_Event_Node* last;

static void push_event(Android_Event* event)
{
	if (last == NULL) 
	{
		first = malloc(sizeof(Android_Event_Node));

		last = first;
	}
	else 
	{
		last->next = malloc(sizeof(Android_Event_Node));

		last = last->next;
	}

	last->event = *event;

	last->next = NULL;
}

static void collect_input_events()
{
	if (input_queue == NULL) 
	{
		return;
	}

	while (AInputQueue_hasEvents(input_queue) > 0) 
	{
		AInputEvent* input_event;

		AInputQueue_getEvent(input_queue, &input_event);

		bool handled = true;

		switch (AInputEvent_getType(input_event)) 
		{
			case AINPUT_EVENT_TYPE_MOTION: 
			{
				int action = AMotionEvent_getAction(input_event) & AMOTION_EVENT_ACTION_MASK;

				Android_Event_Type type = ANDROID_EVENT_UNKNOWN;

				switch (action)
				{
					case AMOTION_EVENT_ACTION_DOWN:
					case AMOTION_EVENT_ACTION_POINTER_DOWN:
					case AMOTION_EVENT_ACTION_BUTTON_PRESS:
					{
						type = ANDROID_EVENT_TOUCH_DOWN;

						break;
					}
					case AMOTION_EVENT_ACTION_UP:
					case AMOTION_EVENT_ACTION_POINTER_UP:
					case AMOTION_EVENT_ACTION_BUTTON_RELEASE:
					case AMOTION_EVENT_ACTION_CANCEL:
					{
						type = ANDROID_EVENT_TOUCH_UP;

						break;
					}
					case AMOTION_EVENT_ACTION_MOVE:
					{
						type = ANDROID_EVENT_TOUCH_MOVE;

						break;
					}
				}

				switch (type)
				{
					case ANDROID_EVENT_TOUCH_DOWN:
					case ANDROID_EVENT_TOUCH_UP:
					{
						int pointer = AMotionEvent_getAction(input_event) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

						int index = AMotionEvent_getPointerId(input_event, pointer);

						float x = AMotionEvent_getX(input_event, pointer);

						float y = AMotionEvent_getY(input_event, pointer);

						push_event(&(Android_Event){ .type = type, .touch_event = { .index = index, .x = x, .y = y } });

						break;
					}
					case ANDROID_EVENT_TOUCH_MOVE:
					{
						int pointer_count = AMotionEvent_getPointerCount(input_event);

						for (int pointer = 0; pointer < pointer_count; pointer++)
						{
							int index = AMotionEvent_getPointerId(input_event, pointer);

							float x = AMotionEvent_getX(input_event, pointer);

							float y = AMotionEvent_getY(input_event, pointer);

							push_event(&(Android_Event){ .type = type, .touch_event = { .index = index, .x = x, .y = y } });
						}

						break;
					}
				}

				break;
			}
			case AINPUT_EVENT_TYPE_KEY:
			{
				int action = AKeyEvent_getAction(input_event);

				Android_Event_Type type = ANDROID_EVENT_UNKNOWN;

				switch (action)
				{
					case AKEY_EVENT_ACTION_DOWN:
					{
						type = ANDROID_EVENT_KEY_DOWN;

						break;
					}
					case AKEY_EVENT_ACTION_UP:
					{
						type = ANDROID_EVENT_KEY_UP;

						break;
					}
				}

				int key_code = AKeyEvent_getKeyCode(input_event);

				Android_Key key = ANDROID_KEY_UNKNOWN;

				// TODO: copy whole enum from keycodes.h?

				switch (key_code)
				{
					case AKEYCODE_VOLUME_DOWN:
					case AKEYCODE_VOLUME_UP:
					{
						handled = false;

						break;
					}
					case AKEYCODE_BACK:
					{
						key = ANDROID_KEY_BACK;

						break;
					}
					case AKEYCODE_ENTER:
					case AKEYCODE_DPAD_CENTER:
					{
						key = ANDROID_KEY_ENTER;

						break;
					}
					case AKEYCODE_SPACE:
					{
						key = ANDROID_KEY_SPACE;

						break;
					}
					case AKEYCODE_DEL:
					{
						key = ANDROID_KEY_BACKSPACE;

						break;
					}
					case AKEYCODE_DPAD_LEFT:
					{
						key = ANDROID_KEY_LEFT;

						break;
					}
					case AKEYCODE_DPAD_RIGHT:
					{
						key = ANDROID_KEY_RIGHT;

						break;
					}
					case AKEYCODE_DPAD_UP:
					{
						key = ANDROID_KEY_UP;

						break;
					}
					case AKEYCODE_DPAD_DOWN:
					{
						key = ANDROID_KEY_DOWN;

						break;
					}
					default:
					{
						if (key_code >= AKEYCODE_0 && key_code <= AKEYCODE_9)
						{
							key = '0' + key_code - AKEYCODE_0;
						}

						if (key_code >= AKEYCODE_A && key_code <= AKEYCODE_Z)
						{
							key = 'A' + key_code - AKEYCODE_A;
						}
					}
				}

				push_event(&(Android_Event){ .type = type, .key_event = { .key = key } });

				break;
			}
		}

		AInputQueue_finishEvent(input_queue, input_event, handled);
	}
}

static void on_destroy(ANativeActivity* activity)
{
	pthread_mutex_lock(&mutex);

	push_event(&(Android_Event){ .type = ANDROID_EVENT_PAUSED });

	pthread_mutex_unlock(&mutex);
}

static void on_pause(ANativeActivity* activity)
{
	pthread_mutex_lock(&mutex);

	push_event(&(Android_Event){ .type = ANDROID_EVENT_PAUSED });

	pthread_mutex_unlock(&mutex);
}

static void on_resume(ANativeActivity* activity)
{
	pthread_mutex_lock(&mutex);

	push_event(&(Android_Event){ .type = ANDROID_EVENT_RESUMED });

	pthread_mutex_unlock(&mutex);
}

static void on_native_window_created(ANativeActivity* activity, ANativeWindow* window)
{
	pthread_mutex_lock(&mutex);

	push_event(&(Android_Event){ .type = ANDROID_EVENT_WINDOW_CREATED, .window_event = { .window = window } });

	pthread_mutex_unlock(&mutex);
}

static void on_native_window_destroyed(ANativeActivity* activity, ANativeWindow* window)
{
	pthread_mutex_lock(&mutex);

	push_event(&(Android_Event){ .type = ANDROID_EVENT_WINDOW_DESTROYED, .window_event = { .window = window } });

	pthread_mutex_unlock(&mutex);
}

static void on_input_queue_created(ANativeActivity* activity, AInputQueue* queue)
{
	pthread_mutex_lock(&mutex);

	input_queue = queue;

	pthread_mutex_unlock(&mutex);
}

static void on_input_queue_destroyed(ANativeActivity* activity, AInputQueue* queue)
{
	pthread_mutex_lock(&mutex);

	input_queue = NULL;

	pthread_mutex_unlock(&mutex);
}

static void entry(ANativeActivity* activity)
{
	platform_init(activity);

	android_main();
}

__attribute__((visibility("default"))) void ANativeActivity_onCreate(ANativeActivity* activity, void* saved_state, size_t saved_state_size)
{
	activity->callbacks->onDestroy = on_destroy;

	activity->callbacks->onPause = on_pause;

	activity->callbacks->onResume = on_resume;

	activity->callbacks->onNativeWindowCreated = on_native_window_created;

	activity->callbacks->onNativeWindowDestroyed = on_native_window_destroyed;

	activity->callbacks->onInputQueueCreated = on_input_queue_created;

	activity->callbacks->onInputQueueDestroyed = on_input_queue_destroyed;

	if (!started)
	{
		started = true;

		pthread_mutex_init(&mutex, NULL);

		pthread_create(&thread, NULL, entry, activity);
	}
}

bool android_poll_event(Android_Event* event)
{
	pthread_mutex_lock(&mutex);

	collect_input_events();

	bool result = first != NULL;

	if (result) 
	{
		*event = first->event;

		Android_Event_Node* next = first->next;

		free(first);

		if (next == NULL)
		{
			first = NULL;

			last = NULL;
		}
		else
		{
			first = next;
		}
	}

	pthread_mutex_unlock(&mutex);

	return result;
}
