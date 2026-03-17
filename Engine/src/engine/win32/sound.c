#include "../sound.h"

#include "platform.h"

Sound* sound_load(const char* path)
{
	return 0;
}

void sound_destroy(Sound* sound)
{
	MCIWndDestroy(sound);
}

bool sound_is_playing(Sound* sound)
{
	return MCIWndGetMode(sound, NULL, 0) == MCI_MODE_PLAY;
}

void sound_play(Sound* sound)
{
	MCIWndHome(sound);

	MCIWndPlay(sound);
}

void sound_stop(Sound* sound)
{
	MCIWndStop(sound);
}

void sound_pause(Sound* sound)
{
	MCIWndPause(sound);
}

void sound_resume(Sound* sound)
{
	MCIWndResume(sound);
}

bool sound_is_looping(Sound* sound)
{
	return MCIWndGetRepeat(sound);
}

void sound_set_looping(Sound* sound, bool looping)
{
	MCIWndSetRepeat(sound, looping);
}

double sound_get_volume(Sound* sound)
{
	return MCIWndGetVolume(sound) / 1000.0;
}

void sound_set_volume(Sound* sound, double volume)
{
	MCIWndSetVolume(sound, volume * 1000);
}

double sound_get_position(Sound* sound)
{
	return MCIWndGetPosition(sound) / 1000.0;
}

void sound_set_position(Sound* sound, double position)
{
	MCIWndSeek(sound, position * 1000);
}
