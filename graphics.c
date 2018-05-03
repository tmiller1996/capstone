#include "graphics.h"
#include "core.h"
#include "rgba.h"
#include "rect.h"
#include "point.h"

#include <libguile.h>
#include <SDL.h>
#include <SDL_image.h>

struct texture_handle {
	SDL_Texture *tex;
	uint32_t format;
	int access;
	int width, height;
};

typedef struct texture_handle texture_handle;

SCM scm_from_tex(texture_handle *tex){
	return scm_from_long((long) tex);
}

texture_handle *scm_to_tex(SCM tex){
	return (texture_handle*) scm_to_long(tex);
}

SCM load_texture(SCM scm_ctx, SCM scm_path){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(!ctx){
		// TODO error
		return scm_from_long(0x0);
	}
	const char *path = scm_to_locale_string(scm_path);
	SDL_Surface *sfc = IMG_Load(path);
	if(!sfc){
		// TODO error
		return scm_from_long(0x0);
	}
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ctx->renderer, sfc);
	if(!tex){
		SDL_FreeSurface(sfc);
		// TODO error
		return scm_from_long(0x0);
	}
	SDL_FreeSurface(sfc);
	return scm_from_long((long) tex);
}

SCM destroy_texture(SCM scm_tex){
	SDL_Texture *tex = (SDL_Texture*) scm_to_long(scm_tex);
	if(tex){
		SDL_DestroyTexture(tex);
	}
	return SCM_BOOL_T;
}

SCM texture_size(SCM scm_tex){
	SDL_Texture *tex = (SDL_Texture*) scm_to_long(scm_tex);
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	return scm_cons(scm_from_int(w), scm_from_int(h));
}


SCM render_clear(SCM scm_ctx, SCM scm_rgba){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	struct rgba rgba;
	if(!scm_to_rgba(scm_rgba, &rgba)){
		// TODO error
		return SCM_BOOL_F;
	}
	if(SDL_SetRenderDrawColor(ctx->renderer, rgba.r, rgba.g, rgba.b, rgba.a) != 0){
		// TODO error
		return SCM_BOOL_F;
	}
	if(SDL_RenderClear(ctx->renderer) != 0){
		// TODO error
		return SCM_BOOL_F;
	}
	return SCM_BOOL_T;
}

SCM render_present(SCM scm_ctx){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	if(ctx){
		SDL_RenderPresent(ctx->renderer);
	}
	return SCM_BOOL_F;
}

SCM render_texture_xy(SCM scm_ctx, SCM scm_tex, SCM scm_xy){
	playctx *ctx = (playctx*) scm_to_long(scm_ctx);
	SDL_Texture *tex = (SDL_Texture*) scm_to_long(scm_tex);
	if(ctx) {
		int access;
		uint32_t format;
		int w, h;
		SDL_QueryTexture(tex, &format, &access, &w, &h);

		SDL_Rect src = {
			.w = w,
			.h = h,
			.x = 0,
			.y = 0,
		};

		SDL_Point point;
		scm_to_point(scm_xy, &point);
		SDL_Rect dst = {
			.w = w,
			.h = h,
			.x = point.x,
			.y = point.y,
		};

		SDL_RenderCopy(ctx->renderer, tex, &src, &dst);
	}
	return SCM_BOOL_F;
}

SCM render_texture_xywh(SCM scm_ctx, SCM scm_tex, SCM scm_xywh){
	playctx *ctx = scm_to_playctx(scm_ctx);
	SDL_Texture *tex = (SDL_Texture*) scm_to_long(scm_tex);
	if(ctx){
		int access;
		uint32_t format;
		int w, h;

		SDL_QueryTexture(tex, &format, &access, &w, &h);

		SDL_Rect src = {
			.w = w,
			.h = h,
			.x = 0,
			.y = 0,
		};

		SDL_Rect dst;
		scm_to_rect(scm_xywh, &dst);
		
		SDL_RenderCopy(ctx->renderer, tex, &src, &dst);
	}
	return SCM_BOOL_F;
}

void init_graphics(){
	scm_c_define_gsubr("render-clear", 2, 0, 0, render_clear);
	scm_c_export("render-clear", NULL);

	scm_c_define_gsubr("render-present", 1, 0, 0, render_present);
	scm_c_export("render-present", NULL);

	scm_c_define_gsubr("load-texture", 2, 0, 0, load_texture);
	scm_c_export("load-texture", NULL);

	scm_c_define_gsubr("destroy-texture", 1, 0, 0, destroy_texture);
	scm_c_export("destroy-texture", NULL);

	scm_c_define_gsubr("texture-size", 1, 0, 0, texture_size);
	scm_c_export("texture-size", NULL);

	scm_c_define_gsubr("render-texture-xy", 3, 0, 0,render_texture_xy);
	scm_c_export("render-texture-xy", NULL);

	scm_c_define_gsubr("render-texture-xywh", 3, 0, 0,render_texture_xywh);
	scm_c_export("render-texture-xywh", NULL);
}