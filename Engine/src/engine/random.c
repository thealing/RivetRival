#include "random.h"

int random_int()
{
	static unsigned int seed = 1;

	seed ^= seed << 13;

	seed ^= seed >> 17;

	seed ^= seed << 5;

	int value = seed & RANDOM_MAX;

	return value;
}

int random_int_below(int x)
{
	return random_int() % x;
}

int random_int_in_range(int l, int h)
{
	return l + random_int_below(h - l + 1);
}

double random_real()
{
	return (double)random_int() / RANDOM_MAX;
}

double random_real_below(double x)
{
	return random_real() * x;
}

double random_real_in_range(double l, double h)
{
	return l + random_real_below(h - l);
}
