#include "input.h"
#include "core.h"
#include "error.h"

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

static void ctx_poll_input(playctx *ctx){
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

#define __SCM_FUNCTION__ "poll-input"
static SCM poll_input(SCM scm_ctx){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		ctx_poll_input(ctx);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "mouse-pos"
static SCM mouse_pos(SCM scm_ctx){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		return scm_cons(scm_from_int(ctx->mouse.x), scm_from_int(ctx->mouse.y));
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

static SCM key_pressed(SCM scm_ctx, SCM scm_key){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		SDL_Keycode key = (SDL_Keycode) scm_to_uint8(scm_key);
		SDL_Scancode scan = SDL_GetScancodeFromKey(key);
		bool current = ctx->keyboard.current ? ctx->keyboard.current[scan] : false;
		bool last = ctx->keyboard.last ? ctx->keyboard.last[scan] : false;
		return current && !last ? SCM_BOOL_T : SCM_BOOL_F;
	} else {
		return SCM_BOOL_F;
	}
}

static SCM key_down(SCM scm_ctx, SCM scm_key){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		SDL_Keycode key = (SDL_Keycode) scm_to_uint8(scm_key);
		SDL_Scancode scan = SDL_GetScancodeFromKey(key);
		bool current = ctx->keyboard.current ? ctx->keyboard.current[scan] : false;
		return current ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return SCM_BOOL_F;
	}
}

static SCM mouse_pressed(SCM scm_ctx, SCM scm_mbutton){
	uint32_t mbutton = scm_to_uint32(scm_mbutton);
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		return ((ctx->mouse.current & SDL_BUTTON(mbutton)) && 
			   !(ctx->mouse.last & SDL_BUTTON(mbutton))) ? 
			   SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return SCM_BOOL_F;
	}
}

static SCM close_requested(SCM scm_ctx){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		return ctx->close_requested ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return SCM_BOOL_F;
	}
}

void init_input(){
	scm_c_define_gsubr("poll-input", 1, 0, 0, poll_input);
	scm_c_export("poll-input", NULL);

	scm_c_define_gsubr("key-pressed", 2, 0, 0, key_pressed);
	scm_c_export("key-pressed", NULL);

	scm_c_define_gsubr("key-down", 2, 0, 0, key_down);
	scm_c_export("key-down", NULL);

	scm_c_define_gsubr("mouse-pressed", 2, 0, 0, mouse_pressed);
	scm_c_export("mouse-pressed", NULL);

	scm_c_define_gsubr("mouse-pos", 1, 0, 0, mouse_pos);
	scm_c_export("mouse-pos", NULL);

	scm_c_define_gsubr("close-requested", 1, 0, 0, close_requested);
	scm_c_export("close-requested", NULL);
}