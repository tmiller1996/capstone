#include "sound.h"
#include "error.h"

#include <libguile.h>
#include <gc.h>
#include <SDL_mixer.h>

struct chunk_handle {
	Mix_Chunk *chunk;
};

struct music_handle {
	Mix_Music *music;
};

static SCM scm_from_music(const music_handle *handle) {
	return scm_from_long((long) handle);
}

static music_handle *scm_to_music(SCM scm_handle) {
	return (music_handle*) scm_to_long(scm_handle);
}

static SCM scm_from_chunk(const chunk_handle *handle) {
	return scm_from_long((long) handle);
}

static chunk_handle *scm_to_chunk(SCM scm_handle) {
	return (chunk_handle*) scm_to_long(scm_handle);
}

static void chunk_finalizer(void *_handle, void *unused){
	chunk_handle *handle = (chunk_handle*) _handle;
	if(handle){
		if(handle->chunk){
			Mix_FreeChunk(handle->chunk);
		}
	}
}

static void music_finalizer(void *_handle, void *unused){
	music_handle *handle = (music_handle*) _handle;
	if(handle){
		if(handle->music){
			Mix_FreeMusic(handle->music);
		}
	}
}

#define __SCM_FUNCTION__ "load-music"
static SCM load_music(SCM scm_path) {
	char *path = scm_to_locale_string(scm_path);
	music_handle *handle = GC_malloc(sizeof(music_handle));
	if(handle){
		Mix_Music *music = Mix_LoadMUS(path);
		if(music){
			handle->music = music;
			GC_register_finalizer(handle, music_finalizer, NULL, 0, 0);
			free(path);
			return scm_from_music(handle);
		}
		else {
			GC_free(handle);
			SCM err = scm_errorstrf("MixLoadMUS returned NULL for '%s' with error: %s", path, Mix_GetError());
			free(path);
			return err;
		}
	}
	else{
		free(path);
		return scm_errorstr("GC_malloc returned NULL");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "load-sound"
static SCM load_sound(SCM scm_path) {
	char *path = scm_to_locale_string(scm_path);
	chunk_handle *handle = GC_malloc(sizeof(chunk_handle));
	if(handle){
		Mix_Chunk *chunk = Mix_LoadWAV(path);
		if(chunk){
			handle->chunk = chunk;
			GC_register_finalizer(handle, chunk_finalizer, NULL, 0, 0);
			free(path);
			return scm_from_chunk(handle);
		}
		else{
			GC_free(handle);
			SCM err = scm_errorstrf("Mix_LoadWAV returned NULL for '%s' with error: %s", path, Mix_GetError());
			free(path);
			return err;
		}
	}
	else{
		free(path);
		return scm_errorstr("GC_malloc returned NULL");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "play-sound"
static SCM play_sound(SCM chunk) {
	chunk_handle *handle = scm_to_chunk(chunk);
	if(handle){
		return scm_from_int(Mix_PlayChannel(-1, handle->chunk, 1));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "play-music"
static SCM play_music(SCM scm_music, SCM scm_loopn){
	music_handle *handle = scm_to_music(scm_music);
	int loopn = scm_to_int(scm_loopn);
	if(handle){
		if(Mix_PlayMusic(handle->music, loopn) == 0){
			return SCM_BOOL_T;
		}
		else{
			return scm_errorstrf("Mix_PlayMusic error: %s", Mix_GetError());
		}
	}
	else{
		return scm_errorstr("invalid music handle");
	}
	return SCM_BOOL_F;
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "loop-music"
static SCM loop_music(SCM scm_music){
	music_handle *handle = scm_to_music(scm_music);
	if(handle){
		if(Mix_PlayMusic(handle->music, -1) == 0){
			return SCM_BOOL_T;
		}
		else{
			return scm_errorstrf("Mix_PlayMusic error: %s", Mix_GetError());
		}
	}
	else{
		return scm_errorstr("invalid music handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "fadein-music"
static SCM fadein_music(SCM music, SCM loops, SCM ms){
	music_handle *handle = scm_to_music(music);
	if(handle){
		if(Mix_FadeInMusic(handle->music, scm_to_int(loops), scm_to_int(ms)) == 0){
			return SCM_BOOL_T;
		}
		else{
			return scm_errorstrf("Mix_FadeInMusic error: %s", Mix_GetError());
		}
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

static SCM fadeout_music(SCM ms){
	return Mix_FadeOutMusic(scm_to_int(ms)) == 1 ? SCM_BOOL_T : SCM_BOOL_F;
}

static SCM music_fading_p(){
	return Mix_FadingMusic() == MIX_NO_FADING ? SCM_BOOL_F : SCM_BOOL_T;
}

static SCM channel_fading_p(SCM channel){
	return Mix_FadingChannel(scm_to_int(channel)) == MIX_NO_FADING ? SCM_BOOL_F : SCM_BOOL_T;
}

static SCM music_playing_p(){
	return Mix_PlayingMusic() ? SCM_BOOL_T : SCM_BOOL_F;
}

static SCM music_paused_p(){
	return Mix_PausedMusic() ? SCM_BOOL_T : SCM_BOOL_F;
}

static SCM halt_music(){
	Mix_HaltMusic();
	return SCM_BOOL_T;
}

static SCM rewind_music(){
	Mix_RewindMusic();
	return SCM_BOOL_T;
}

static SCM pause_music(){
	Mix_PauseMusic();
	return SCM_BOOL_T;
}

static SCM resume_music(){
	Mix_ResumeMusic();
	return SCM_BOOL_T;
}

static SCM music_volume(SCM volume){
	return scm_from_int(Mix_VolumeMusic(scm_to_int(volume)));
}

static SCM resume_channel(SCM channel){
	Mix_Resume(scm_to_int(channel));
	return SCM_BOOL_T;
}

static SCM pause_channel(SCM channel){
	Mix_Pause(scm_to_int(channel));
	return SCM_BOOL_T;
}

static SCM halt_channel(SCM channel){
	Mix_HaltChannel(scm_to_int(channel));
	return SCM_BOOL_T;
}

static SCM expire_channel(SCM channel, SCM ticks){
	Mix_ExpireChannel(scm_to_int(channel), scm_to_int(ticks));
	return SCM_BOOL_T;
}

static SCM channel_volume(SCM channel, SCM volume){
	return scm_from_int(Mix_Volume(scm_to_int(channel), scm_to_int(volume)));
}

static SCM allocate_channels(SCM channels){
	return scm_from_int(Mix_AllocateChannels(scm_to_int(channels)));
}

#define __SCM_FUNCTION__ "play-channel"
static SCM play_channel(SCM scm_channel, SCM scm_handle, SCM loops){
	chunk_handle *handle = scm_to_chunk(scm_handle);
	if(handle){
		int channel = Mix_PlayChannel(scm_to_int(scm_channel), handle->chunk, scm_to_int(loops));
		if(channel != -1){
			return scm_from_int(channel);
		}
		else{
			return scm_errorstrf("Mix_PlayChannel error: %s", Mix_GetError());
		}
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

static SCM channel_paused_p(SCM channel){
	return scm_from_int(Mix_Paused(scm_to_int(channel)));
}

static SCM channel_playing_p(SCM channel){
	return scm_from_int(Mix_Playing(scm_to_int(channel)));
}

#define __SCM_FUNCTION__ "fadein-channel"
static SCM fadein_channel(SCM channel, SCM chunk, SCM loops, SCM ms){
	chunk_handle *handle = scm_to_chunk(chunk);
	if(handle){
		int retval = Mix_FadeInChannel(scm_to_int(channel), handle->chunk, scm_to_int(loops), scm_to_int(ms));
		if(retval != -1){
			return scm_from_int(retval);
		}
		else{
			return scm_errorstrf("Mix_FadeInChannel error: %s", Mix_GetError());
		}
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

static SCM fadeout_channel(SCM channel, SCM ms){
	return scm_from_int(Mix_FadeOutChannel(scm_to_int(channel), scm_to_int(ms)));
}

void init_sound(){
	Mix_Init(0);

	scm_c_define_gsubr("load-music", 1, 0, 0, load_music);
	scm_c_export("load-music", NULL);

	scm_c_define_gsubr("load-sound", 1, 0, 0, load_sound);
	scm_c_export("load-sound", NULL);

	scm_c_define_gsubr("play-music", 2, 0, 0, play_music);
	scm_c_export("play-music", NULL);

	scm_c_define_gsubr("loop-music", 1, 0, 0, loop_music);
	scm_c_export("loop-music", NULL);

	scm_c_define_gsubr("play-sound", 1, 0, 0, play_sound);
	scm_c_export("play-sound", NULL);

	scm_c_define_gsubr("fadein-music", 3, 0, 0, fadein_music);
	scm_c_export("fadein-music", NULL);

	scm_c_define_gsubr("fadeout-music", 1, 0, 0, fadeout_music);
	scm_c_export("fadeout-music", NULL);

	scm_c_define_gsubr("music-fading-p", 0, 0, 0, music_fading_p);
	scm_c_export("music-fading-p");

	scm_c_define_gsubr("music-playing-p", 0, 0, 0, music_playing_p);
	scm_c_export("music-playing-p");

	scm_c_define_gsubr("music-paused-p", 0, 0, 0, music_paused_p);
	scm_c_export("music-paused-p");

	scm_c_define_gsubr("halt-music", 0, 0, 0, halt_music);
	scm_c_export("halt-music");

	scm_c_define_gsubr("rewind-music", 0, 0, 0, rewind_music);
	scm_c_export("rewind-music");

	scm_c_define_gsubr("pause-music", 0, 0, 0, pause_music);
	scm_c_export("pause-music");

	scm_c_define_gsubr("resume-music", 0, 0, 0, resume_music);
	scm_c_export("resume-music");

	scm_c_define_gsubr("music-volume", 1, 0, 0, music_volume);
	scm_c_export("music-volume");

	scm_c_define_gsubr("resume-channel", 1, 0, 0, resume_channel);
	scm_c_export("resume-channel");

	scm_c_define_gsubr("pause-channel", 1, 0, 0, pause_channel);
	scm_c_export("pause-channel");

	scm_c_define_gsubr("halt-channel", 1, 0, 0, halt_channel);
	scm_c_export("halt-channel");

	scm_c_define_gsubr("expire-channel", 2, 0, 0, expire_channel);
	scm_c_export("expire-channel");

	scm_c_define_gsubr("channel-volume", 2, 0, 0, channel_volume);
	scm_c_export("channel-volume");

	scm_c_define_gsubr("allocate-channels", 1, 0, 0, allocate_channels);
	scm_c_export("allocate-channels");

	scm_c_define_gsubr("channel-paused-p", 1, 0, 0, channel_paused_p);
	scm_c_export("channel-paused-p");

	scm_c_define_gsubr("channel-playing-p", 1, 0, 0, channel_playing_p);
	scm_c_export("channel-playing-p");

	scm_c_define_gsubr("play-channel", 3, 0, 0, play_channel);
	scm_c_export("play-channel");

	scm_c_define_gsubr("channel-fading-p", 1, 0, 0, channel_fading_p);
	scm_c_export("channel-fading-p");

	scm_c_define_gsubr("fadein-channel", 4, 0, 0, fadein_channel);
	scm_c_export("fadein-channel");

	scm_c_define_gsubr("fadeout-channel", 2, 0, 0, fadeout_channel);
	scm_c_export("fadeout-channel");
}