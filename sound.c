#include "sound.h"

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

SCM load_music(SCM scm_path) {
	char *path = scm_to_locale_string(scm_path);
	music_handle *handle = GC_malloc(sizeof(music_handle));
	if(handle){
		Mix_Music *music = Mix_LoadMUS(path);
		if(music){
			handle->music = music;
			GC_register_finalizer(handle, music_finalizer, NULL, 0, 0);
			return scm_from_music(handle);
		}
		else {
			GC_free(handle);
			return SCM_BOOL_F;
		}
	}
	else{
		return SCM_BOOL_F;
	}
}

SCM load_chunk(SCM scm_path) {
	char *path = scm_to_locale_string(scm_path);
	chunk_handle *handle = GC_malloc(sizeof(chunk_handle));
	if(handle){
		Mix_Chunk *chunk = Mix_LoadWAV(path);
		if(chunk){
			handle->chunk = chunk;
			GC_register_finalizer(handle, chunk_finalizer, NULL, 0, 0);
			return scm_from_chunk(handle);
		}
		else{
			GC_free(handle);
			return SCM_BOOL_F;
		}
	}
	else{
		return SCM_BOOL_F;
	}
}

void init_sound() {

}