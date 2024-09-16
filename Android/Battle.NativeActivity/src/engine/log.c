#include "log.h"

#include "platform.h"

void log_info(const char* format, ...)
{
	va_list args;

	va_start(args, format);

	__android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, args);

	va_end(args);
}

void log_warning(const char* format, ...)
{
	va_list args;

	va_start(args, format);

	__android_log_vprint(ANDROID_LOG_WARN, LOG_TAG, format, args);

	va_end(args);
}

void log_error(const char* format, ...)
{
	va_list args;

	va_start(args, format);

	__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, args);

	va_end(args);
}
