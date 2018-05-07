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

SCM scm_from_music(const music_handle *handle) {
	return scm_from_long((long) handle);
}

music_handle *scm_to_music(SCM scm_handle) {
	return (music_handle*) scm_to_long(scm_handle);
}

SCM scm_from_chunk(const chunk_handle *handle) {
	return scm_from_long((long) handle);
}

chunk_handle *scm_to_chunk(SCM scm_handle) {
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
SCM load_music(SCM scm_path) {
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
SCM load_sound(SCM scm_path) {
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

SCM play_sound(SCM scm_chunk) {
	// TODO
	return SCM_BOOL_F;
}

SCM play_music(SCM scm_music){
	// TODO
	return SCM_BOOL_F;
}

void init_sound(){
	Mix_Init(0);

	scm_c_define_gsubr("load-music", 1, 0, 0, load_music);
	scm_c_export("load-music", NULL);

	scm_c_define_gsubr("load-sound", 1, 0, 0, load_sound);
	scm_c_export("load-sound", NULL);

	scm_c_define_gsubr("play-music", 1, 0, 0, load_music);
	scm_c_export("play-music", NULL);

	scm_c_define_gsubr("play-sound", 1, 0, 0, play_sound);
	scm_c_export("play-sound", NULL);
}