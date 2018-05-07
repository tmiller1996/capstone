#include "graphics.h"
#include "core.h"
#include "rgba.h"
#include "rect.h"
#include "point.h"
#include "error.h"

#include <libguile.h>
#include <gc.h>
#include <SDL.h>
#include <SDL_image.h>

struct texture_handle {
	SDL_Texture *tex;
	uint32_t format;
	int access;
	int w, h;
};

typedef struct texture_handle texture_handle;

SCM scm_from_tex(texture_handle *tex){
	return scm_from_long((long) tex);
}

texture_handle *scm_to_tex(SCM tex){
	return (texture_handle*) scm_to_long(tex);
}

static void texture_finalizer(void *_handle, void *unused){
	texture_handle *handle = (texture_handle*) _handle;
	if(handle){
		SDL_DestroyTexture(handle->tex);
	}
}

static texture_handle *create_handle(SDL_Texture *tex){
	texture_handle *handle = GC_malloc(sizeof(texture_handle));
	if(handle){
		GC_register_finalizer(handle, NULL, texture_finalizer, 0, 0);
		handle->tex = tex;
		SDL_QueryTexture(handle->tex, &handle->format, &handle->access, &handle->w, &handle->h);
		return handle;
	}
	else{
		return NULL;
	}
}

SCM texture_from_surface(const char *__SCM_FUNCTION__, SDL_Renderer *renderer, SDL_Surface *surface){
	if(renderer){
		if(surface){
			SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
			if(tex){
				texture_handle *handle = create_handle(tex);
				if(handle){
					return scm_from_tex(handle);
				}
				else{
					SDL_DestroyTexture(tex);
					return scm_errorstr("create_handle returned NULL");
				}
			}
			else{
				return scm_errorstrf("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
			}
		}
		else{
			return scm_errorstr("invalid surface");
		}
	}
	else{
		return scm_errorstr("invalid renderer");
	}
}

#define __SCM_FUNCTION__ "load-texture"
static SCM load_texture(SCM scm_ctx, SCM scm_path){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		char *path = scm_to_locale_string(scm_path);
		SDL_Surface *sfc = IMG_Load(path);
		if(sfc){
			SDL_Texture *tex = SDL_CreateTextureFromSurface(ctx->renderer, sfc);
			if(tex){
				texture_handle *handle = create_handle(tex);
				if(handle){
					free(path);
					SDL_FreeSurface(sfc);
					return scm_from_tex(handle);
				}
				else{
					SCM err = scm_errorstr("GC_malloc returned NULL");
					SDL_DestroyTexture(tex);
					SDL_FreeSurface(sfc);
					free(path);
					return err;
				}
			}
			else {
				SCM err = scm_errorstrf("SCL_CreateTextureFromSurface Error: %s\n, Path: %s", SDL_GetError(), path);
				SDL_FreeSurface(sfc);
				free(path);
				return err;
			}
		}
		else{
			SCM err = scm_errorstrf("IMG_Load Error: %s, Path: %s", SDL_GetError(), path);
			free(path);
			return err;
		}
	}
	else{
		SCM err = scm_errorstr("Invalid Play Context");
		return err;
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "texture-size"
static SCM texture_size(SCM scm_tex){
	texture_handle *handle = scm_to_tex(scm_tex);
	return scm_cons(scm_from_int(handle->w), scm_from_int(handle->h));
}
#undef __SCM_FUNCTION__


#define __SCM_FUNCTION__ "render-clear"
static SCM render_clear(SCM scm_ctx, SCM scm_rgba){
	playctx *ctx = scm_to_playctx(scm_ctx);
	SDL_Color rgba;
	if(scm_to_rgba(scm_rgba, &rgba)){
		if(SDL_SetRenderDrawColor(ctx->renderer, rgba.r, rgba.g, rgba.b, rgba.a) == 0){
			if(SDL_RenderClear(ctx->renderer) == 0){
				return SCM_BOOL_T;
			}
			else{
				return scm_errorstrf("SDL_RenderClear error: %s", SDL_GetError());
			}
		}
		else{
			return scm_errorstrf("SDL_SetRenderDrawColor error: %s", SDL_GetError());
		}
	}
	else{
		return scm_errorstr("Expected RGBA value in list form");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "render-present"
static SCM render_present(SCM scm_ctx){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		SDL_RenderPresent(ctx->renderer);
	}
	return SCM_BOOL_F;
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "render-texture-xy"
static SCM render_texture_xy(SCM scm_ctx, SCM scm_tex, SCM scm_xy){
	playctx *ctx = scm_to_playctx(scm_ctx);
	texture_handle *handle = scm_to_tex(scm_tex);
	if(ctx && handle) {
		SDL_Rect src = {
			.w = handle->w,
			.h = handle->h,
			.x = 0,
			.y = 0,
		};

		SDL_Point point;
		scm_to_point(scm_xy, &point);
		SDL_Rect dst = {
			.w = handle->w,
			.h = handle->h,
			.x = point.x,
			.y = point.y,
		};

		SDL_RenderCopy(ctx->renderer, handle->tex, &src, &dst);
	}
	return SCM_BOOL_F;
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "render-texture-xywh"
static SCM render_texture_xywh(SCM scm_ctx, SCM scm_tex, SCM scm_xywh){
	playctx *ctx = scm_to_playctx(scm_ctx);
	texture_handle *handle = scm_to_tex(scm_tex);
	if(ctx && handle){

		SDL_Rect src = {
			.w = handle->w,
			.h = handle->h,
			.x = 0,
			.y = 0,
		};

		SDL_Rect dst;
		scm_to_rect(scm_xywh, &dst);
		
		SDL_RenderCopy(ctx->renderer, handle->tex, &src, &dst);
	}
	return SCM_BOOL_F;
}
#undef __SCM_FUNCTION__

void init_graphics(){
	scm_c_define_gsubr("render-clear", 2, 0, 0, render_clear);
	scm_c_export("render-clear", NULL);

	scm_c_define_gsubr("render-present", 1, 0, 0, render_present);
	scm_c_export("render-present", NULL);

	scm_c_define_gsubr("load-texture", 2, 0, 0, load_texture);
	scm_c_export("load-texture", NULL);

	scm_c_define_gsubr("texture-size", 1, 0, 0, texture_size);
	scm_c_export("texture-size", NULL);

	scm_c_define_gsubr("render-texture-xy", 3, 0, 0,render_texture_xy);
	scm_c_export("render-texture-xy", NULL);

	scm_c_define_gsubr("render-texture-xywh", 3, 0, 0,render_texture_xywh);
	scm_c_export("render-texture-xywh", NULL);
}