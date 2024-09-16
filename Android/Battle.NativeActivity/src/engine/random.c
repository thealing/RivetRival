#include "random.h"

#include "platform.h"

int random_int()
{
	return arc4random() & RANDOM_MAX;
}

int random_int_below(int x)
{
	return arc4random_uniform(x);
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
