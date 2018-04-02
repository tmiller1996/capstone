#include <stdbool.h>

#include <libguile.h>
#include <SDL.h>

struct play_context {
	SDL_Window *window;
	SDL_Renderer *renderer;
};

SCM	create_context(SCM x){
	struct play_context *ctx = malloc(sizeof(struct play_context));
	if(!ctx){
		// TODO error here
		return scm_from_long(0x0);
	}
	ctx->window = SDL_CreateWindow("sample pls replace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if(!ctx->window){
		free(ctx);
		// TODO error here
		return scm_from_long(0x0);
	}
	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
	if(!ctx->renderer){
		SDL_DestroyWindow(ctx->window);
		free(ctx);
		// TODO error here
		return scm_from_long(0x0);
	}
	return scm_from_long((long) ctx);
}

SCM destroy_context(SCM x){
	struct play_context *ctx = (struct play_context*) scm_to_long(x);
	if(ctx->renderer){
		SDL_DestroyRenderer(ctx->renderer);
	}
	if(ctx->window){
		SDL_DestroyWindow(ctx->window);
	}
	free(ctx);
	return scm_from_bool(true);
}

void init_play_core(void *unused){
	scm_c_define_gsubr("play-create-context", 1, 0, 0, create_context);
	scm_c_export("play-create-context", NULL);

	scm_c_define_gsubr("play-destroy-context", 1, 0, 0, destroy_context);
	scm_c_export("play-destroy-context", NULL);
}