#pragma once

#include <string.h>

int data_write(const char* path, void* buffer, int buffer_size);

int data_read(const char* path, void* buffer, int buffer_size);
