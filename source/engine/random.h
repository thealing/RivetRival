#pragma once

#include <stdlib.h>

#include <limits.h>

#define RANDOM_MAX INT_MAX

int random_int();

int random_int_below(int x);

int random_int_in_range(int l, int h);

double random_real();

double random_real_below(double x);

double random_real_in_range(double l, double h);
