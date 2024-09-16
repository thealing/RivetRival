#include "sound.h"

#include "platform.h"

struct Sound
{
	SLObjectItf player_obj;

	SLPlayItf play_itf;

	SLSeekItf seek_itf;

	SLVolumeItf volume_itf;
};

static void player_callback(SLPlayItf caller, void *context, SLuint32 event)
{
	(*caller)->SetPlayState(caller, SL_PLAYSTATE_STOPPED);
}

Sound* sound_load(const char* path)
{
	static SLObjectItf engine_obj;

	static SLEngineItf engine_itf;

	static SLObjectItf output_mix_obj;

	if (engine_obj == NULL)
	{
		slCreateEngine(&engine_obj, 0, NULL, 0, NULL, NULL);

		(*engine_obj)->Realize(engine_obj, SL_BOOLEAN_FALSE);

		(*engine_obj)->GetInterface(engine_obj, SL_IID_ENGINE, &engine_itf);

		(*engine_itf)->CreateOutputMix(engine_itf, &output_mix_obj, 0, NULL, NULL);

		(*output_mix_obj)->Realize(output_mix_obj, SL_BOOLEAN_FALSE);
	}

	AAsset* asset = AAssetManager_open(asset_manager, path, AASSET_MODE_UNKNOWN);

	off_t start;

	off_t length;

	int fd = AAsset_openFileDescriptor(asset, &start, &length);

	AAsset_close(asset);

	SLDataLocator_AndroidFD source_locator = { SL_DATALOCATOR_ANDROIDFD, fd, start, length };

	SLDataFormat_MIME source_format = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };

	SLDataSource source = { &source_locator, &source_format };

	SLDataLocator_OutputMix sink_locator = { SL_DATALOCATOR_OUTPUTMIX, output_mix_obj };

	SLDataSink sink = { &sink_locator, NULL };

	const SLInterfaceID ids[3] = { SL_IID_PLAY, SL_IID_SEEK, SL_IID_VOLUME };

	const SLboolean req[3] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

	SLObjectItf player_obj;

	SLPlayItf play_itf;

	SLSeekItf seek_itf;

	SLVolumeItf volume_itf;

	(*engine_itf)->CreateAudioPlayer(engine_itf, &player_obj, &source, &sink, 3, ids, req);

	(*player_obj)->Realize(player_obj, SL_BOOLEAN_FALSE);

	(*player_obj)->GetInterface(player_obj, SL_IID_PLAY, &play_itf);

	(*player_obj)->GetInterface(player_obj, SL_IID_SEEK, &seek_itf);

	(*player_obj)->GetInterface(player_obj, SL_IID_VOLUME, &volume_itf);

	(*play_itf)->RegisterCallback(play_itf, player_callback, NULL);

	(*play_itf)->SetCallbackEventsMask(play_itf, SL_PLAYEVENT_HEADATEND);

	Sound* sound = malloc(sizeof(Sound));

	sound->player_obj = player_obj;

	sound->play_itf = play_itf;

	sound->seek_itf = seek_itf;

	sound->volume_itf = volume_itf;

	return sound;
}

void sound_destroy(Sound* sound)
{
	(*sound->player_obj)->Destroy(sound->player_obj);
	
	free(sound);
}

bool sound_is_playing(Sound* sound)
{
	SLuint32 play_state;

	(*sound->play_itf)->GetPlayState(sound->play_itf, &play_state);

	return play_state == SL_PLAYSTATE_PLAYING;
}

void sound_play(Sound* sound)
{
	(*sound->seek_itf)->SetPosition(sound->seek_itf, 0, SL_SEEKMODE_ACCURATE);

	(*sound->play_itf)->SetPlayState(sound->play_itf, SL_PLAYSTATE_PLAYING);
}

void sound_stop(Sound* sound)
{
	(*sound->play_itf)->SetPlayState(sound->play_itf, SL_PLAYSTATE_STOPPED);
}

void sound_pause(Sound* sound)
{
	SLuint32 play_state;

	(*sound->play_itf)->GetPlayState(sound->play_itf, &play_state);

	if (play_state == SL_PLAYSTATE_PLAYING)
	{
		(*sound->play_itf)->SetPlayState(sound->play_itf, SL_PLAYSTATE_PAUSED);
	}
}

void sound_resume(Sound* sound)
{
	SLuint32 play_state;

	(*sound->play_itf)->GetPlayState(sound->play_itf, &play_state);

	if (play_state == SL_PLAYSTATE_PAUSED)
	{
		(*sound->play_itf)->SetPlayState(sound->play_itf, SL_PLAYSTATE_PLAYING);
	}
}

bool sound_is_looping(Sound* sound)
{
	SLboolean enabled;

	SLmillisecond start;

	SLmillisecond end;

	(*sound->seek_itf)->GetLoop(sound->seek_itf, &enabled, &start, &end);

	return enabled;
}

void sound_set_looping(Sound* sound, bool looping)
{
	(*sound->seek_itf)->SetLoop(sound->seek_itf, looping, 0, SL_TIME_UNKNOWN);
}

double sound_get_position(Sound* sound)
{
	SLmillisecond position;

	(*sound->play_itf)->GetPosition(sound->play_itf, &position);

	return position / 1000.0;
}

void sound_set_position(Sound* sound, double position)
{
	(*sound->seek_itf)->SetPosition(sound->seek_itf, position * 1000.0, SL_SEEKMODE_ACCURATE);
}

double sound_get_volume(Sound* sound)
{
	SLmillibel volume;

	(*sound->volume_itf)->GetVolumeLevel(sound->volume_itf, &volume);

	return pow(10.0, volume / 2000.0);
}

void sound_set_volume(Sound* sound, double volume)
{
	(*sound->volume_itf)->SetVolumeLevel(sound->volume_itf, fmax(log10(volume) * 2000.0, SL_MILLIBEL_MIN));
}
