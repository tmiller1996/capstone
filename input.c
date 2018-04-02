#include "input.h"
#include "core.h"

static void ctx_poll_mouse(playctx *ctx){
	uint32_t state = SDL_GetMouseState(&ctx->mouse.x, &ctx->mouse.y);
	ctx->mouse.last = ctx->mouse.current;
	ctx->mouse.current = state;
}

static void ctx_poll_keyboard(playctx *ctx){
	const uint8_t *keystate = SDL_GetKeyboardState(&ctx->keyboard.numkeys);
	if(!ctx->keyboard.current){
		ctx->keyboard.current = calloc(sizeof(uint8_t), ctx->keyboard.numkeys);
	}
	if(!ctx->keyboard.last){
		ctx->keyboard.last = calloc(sizeof(uint8_t), ctx->keyboard.numkeys);
	}
	for(int i = 0; i < ctx->keyboard.numkeys; ++i){
		ctx->keyboard.last[i] = ctx->keyboard.current[i];
		ctx->keyboard.current[i] = keystate[i];
	}
}

void ctx_poll_input(playctx *ctx){
	SDL_Event event;
	SDL_PumpEvents();
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			ctx->close_requested = true;
		}
	}
	ctx_poll_keyboard(ctx);
	ctx_poll_mouse(ctx);
}