#include "bot.h"

Bot* bot_create()
{
	Bot* bot = calloc(1, sizeof(Bot));

	return bot;
}

void bot_destroy(Bot* bot)
{
	free(bot);
}

void bot_reset(Bot* bot)
{
	bot->forward = false;

	bot->backward = false;

	bot->update_time = 0.4;
}

void bot_update(Bot* bot, bool in_air, double bot_location, double bot_angle, Vector bot_velocity, double enemy_location, double time)
{
	if (time > bot->update_time)
	{
		if (enemy_location > bot_location)
		{
			bot->forward = true;

			bot->backward = false;
		}

		if (enemy_location < bot_location)
		{
			bot->forward = false;

			bot->backward = true;
		}

		if (bot->forward && bot_angle > M_PI * 0.3)
		{
			bot->forward = false;

			bot->backward = true;
		}

		if (bot->backward && bot_angle < -M_PI * 0.3)
		{
			bot->forward = true;

			bot->backward = false;
		}

		if (fabs(bot_location - enemy_location) < 250 && random_below(6) == 0)
		{
			bot->forward ^= bot->backward;

			bot->backward ^= bot->forward;

			bot->forward ^= bot->backward;
		}

		bot->update_time = time + random_fraction_in_range(0.2, 1);
	}

	if (in_air)
	{
		double threshold = bot_velocity.y > 0 ? 0.4 : 0.2;

		if (bot_velocity.x > 0 && bot_angle < M_PI * threshold)
		{
			bot->forward = true;

			bot->backward = false;
		}

		if (bot_velocity.x < 0 && bot_angle > -M_PI * threshold)
		{
			bot->forward = false;

			bot->backward = true;
		}
	}

	if (bot_angle > M_PI * 0.45)
	{
		bot->forward = false;

		bot->backward = true;
	}

	if (bot_angle < -M_PI * 0.45)
	{
		bot->forward = true;

		bot->backward = false;
	}
}
