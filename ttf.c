#include "ttf.h"
#include "error.h"
#include "core.h"
#include "rgba.h"
#include "graphics.h"

#include <SDL_ttf.h>
#include <gc.h>
#include <libguile.h>

struct font_handle{
	TTF_Font *font;
};

static font_handle *scm_to_font(SCM scm){
	return (font_handle*) scm_to_long(scm);
}

static SCM scm_from_font(font_handle *handle){
	return scm_from_long((long) handle);
}

static void font_finalizer(void *_handle, void *unused){
	font_handle *handle = (font_handle*) _handle;
	if(handle){
		if(handle->font){
			TTF_CloseFont(handle->font);
		}
	}
}

#define __SCM_FUNCTION__ "open-font"
static SCM open_font(SCM scm_font, SCM size){
	char *path = scm_to_locale_string(scm_font);
	TTF_Font *font = TTF_OpenFont(path, scm_to_int(size));
	if(font){
		font_handle *handle = GC_malloc(sizeof(font_handle));
		if(handle){
			handle->font = font;
			GC_register_finalizer(handle, font_finalizer, NULL, 0, 0);
			return scm_from_font(handle);
		}
		else{
			TTF_CloseFont(font);
			free(path);
			return scm_errorstr("GC_malloc returned NULL");
		}
	}
	else{
		SCM err = scm_errorstrf("TTF_OpenFont returned NULL for '%s' with error: ", path, TTF_GetError());
		free(path);
		return err;
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-bold-p"
static SCM font_bold_p(SCM font){
	font_handle *handle  = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		return style & TTF_STYLE_BOLD ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-italic-p"
static SCM font_italic_p(SCM font){
	font_handle *handle  = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		return style & TTF_STYLE_ITALIC ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-underline-p"
static SCM font_underline_p(SCM font){
	font_handle *handle  = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		return style & TTF_STYLE_UNDERLINE ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-strikethrough-p"
static SCM font_strikethrough_p(SCM font){
	font_handle *handle  = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		return style & TTF_STYLE_STRIKETHROUGH ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-normal-p"
static SCM font_normal_p(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		if((style & TTF_STYLE_BOLD) || (style & TTF_STYLE_ITALIC) || 
			(style & TTF_STYLE_STRIKETHROUGH) || (style & TTF_STYLE_UNDERLINE)){
			return SCM_BOOL_F;
		}
		else{
			return SCM_BOOL_T;
		}
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-bold"
static SCM font_bold(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		TTF_SetFontStyle(handle->font, style | TTF_STYLE_BOLD);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-italic"
static SCM font_italic(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		TTF_SetFontStyle(handle->font, style | TTF_STYLE_ITALIC);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-underline"
static SCM font_underline(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		TTF_SetFontStyle(handle->font, style | TTF_STYLE_UNDERLINE);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-strikethrough"
static SCM font_strikethrough(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		int style = TTF_GetFontStyle(handle->font);
		TTF_SetFontStyle(handle->font, style | TTF_STYLE_STRIKETHROUGH);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-normal"
static SCM font_normal(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		TTF_SetFontStyle(handle->font, 0);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-ascent"
static SCM font_ascent(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_int(TTF_FontAscent(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-descent"
static SCM font_descent(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_int(TTF_FontDescent(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-height"
static SCM font_height(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_int(TTF_FontHeight(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-faces"
static SCM font_faces(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_int(TTF_FontFaces(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-kerning-p"
static SCM font_kerning_p(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return TTF_GetFontKerning(handle->font) != 0 ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-kerning"
static SCM font_kerning(SCM font, SCM p){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		TTF_SetFontKerning(handle->font, scm_is_true(p) ? 1 : 0);
		return SCM_BOOL_T;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-lineskip"
static SCM font_lineskip(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_int(TTF_FontLineSkip(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-outline"
static SCM font_outline(SCM font, SCM outline){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		if(outline == SCM_UNDEFINED){
			return scm_from_int(TTF_GetFontOutline(handle->font));
		}
		else{
			TTF_SetFontOutline(handle->font, scm_to_int(outline));
			return SCM_BOOL_T;
		}
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-style-name"
static SCM font_style_name(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_locale_string(TTF_FontFaceStyleName(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-family-name"
static SCM font_family_name(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return scm_from_locale_string(TTF_FontFaceFamilyName(handle->font));
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-fixed-width-p"
static SCM font_fixed_width_p(SCM font){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		return TTF_FontFaceIsFixedWidth(handle->font) != 0 ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-provides-glyph-p"
static SCM font_provides_glyph_p(SCM font, SCM scm_glyph){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
		return TTF_GlyphIsProvided(handle->font, glyph) != 0 ? SCM_BOOL_T : SCM_BOOL_F;
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-glyph-minx"
static SCM font_glyph_minx(SCM font, SCM scm_glyph){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
		int minx;
		TTF_GlyphMetrics(handle->font, glyph, &minx, NULL, NULL, NULL, NULL);
		return scm_from_int(minx);
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-glyph-maxx"
static SCM font_glyph_maxx(SCM font, SCM scm_glyph){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
		int maxx;
		TTF_GlyphMetrics(handle->font, glyph, NULL, &maxx, NULL, NULL, NULL);
		return scm_from_int(maxx);
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-glyph-miny"
static SCM font_glyph_miny(SCM font, SCM scm_glyph){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
		int miny;
		TTF_GlyphMetrics(handle->font, glyph, NULL, NULL, &miny, NULL, NULL);
		return scm_from_int(miny);
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-glyph-maxy"
static SCM font_glyph_maxy(SCM font, SCM scm_glyph){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
		int maxy;
		TTF_GlyphMetrics(handle->font, glyph, NULL, NULL, NULL, &maxy, NULL);
		return scm_from_int(maxy);
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-glyph-advance"
static SCM font_glyph_advance(SCM font, SCM scm_glyph){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
		int advance;
		TTF_GlyphMetrics(handle->font, glyph, NULL, NULL, NULL, NULL, &advance);
		return scm_from_int(advance);
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-size-text"
static SCM font_size_text(SCM font, SCM scm_text){
	font_handle *handle = scm_to_font(font);
	if(handle && handle->font){
		char *text = scm_to_locale_string(scm_text);
		int w, h;
		if(TTF_SizeText(handle->font, text, &w, &h)){
			return scm_cons(scm_from_int(w), scm_from_int(h));
		}
		else{
			free(text);
			return scm_errorstrf("TTF_SizeText error: %s", TTF_GetError());
		}
	}
	else{
		return scm_errorstr("invalid handle");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-render-text-solid"
static SCM font_render_text_solid(SCM scm_ctx, SCM font, SCM scm_text, SCM scm_color){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		font_handle *handle = scm_to_font(font);
		if(handle && handle->font){
			char *text = scm_to_locale_string(scm_text);
			SDL_Color color;
			if(scm_to_rgba(scm_color, &color)){
				SDL_Surface *sfc = TTF_RenderText_Solid(handle->font, text, color);
				if(sfc){
					free(text);
					return texture_from_surface(__SCM_FUNCTION__, ctx->renderer, sfc);
				}
				else{
					free(text);
					return scm_errorstrf("TTF_RenderText_Solid error: %s", TTF_GetError());
				}
			}
			else{
				free(text);
				return scm_errorstr("expected color");
			}
		}
		else{
			return scm_errorstr("invalid handle");
		}
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-render-glyph-solid"
static SCM font_render_glyph_solid(SCM scm_ctx, SCM font, SCM scm_glyph, SCM scm_color){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		font_handle *handle = scm_to_font(font);
		if(handle && handle->font){
			uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
			SDL_Color color;
			if(scm_to_rgba(scm_color, &color)){
				SDL_Surface *sfc = TTF_RenderGlyph_Solid(handle->font, glyph, color);
				if(sfc){
					return texture_from_surface(__SCM_FUNCTION__, ctx->renderer, sfc);
				}
				else{
					return scm_errorstrf("TTF_RenderGlyph_Solid error: %s", TTF_GetError());
				}
			}
			else{
				return scm_errorstr("expected color");
			}
		}
		else{
			return scm_errorstr("invalid handle");
		}
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-render-text-shaded"
static SCM font_render_text_shaded(SCM scm_ctx, SCM font, SCM scm_text, SCM scm_fg, SCM scm_bg){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		font_handle *handle = scm_to_font(font);
		if(handle && handle->font){
			char *text = scm_to_locale_string(scm_text);
			SDL_Color fg, bg;
			if(scm_to_rgba(scm_fg, &fg) || scm_to_rgba(scm_bg, &bg)){
				SDL_Surface *sfc = TTF_RenderText_Shaded(handle->font, text, fg, bg);
				if(sfc){
					free(text);
					return texture_from_surface(__SCM_FUNCTION__, ctx->renderer, sfc);
				}
				else{
					free(text);
					return scm_errorstrf("TTF_RenderText_Shaded error: %s", TTF_GetError());
				}
			}
			else{
				free(text);
				return scm_errorstr("expected color");
			}
		}
		else{
			return scm_errorstr("invalid handle");
		}
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-render-glyph-shaded"
static SCM font_render_glyph_shaded(SCM scm_ctx, SCM font, SCM scm_glyph, SCM scm_fg, SCM scm_bg){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		font_handle *handle = scm_to_font(font);
		if(handle && handle->font){
			uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
			SDL_Color fg, bg;
			if(scm_to_rgba(scm_fg, &fg) || scm_to_rgba(scm_bg, &bg)){
				SDL_Surface *sfc = TTF_RenderGlyph_Shaded(handle->font, glyph, fg, bg);
				if(sfc){
					return texture_from_surface(__SCM_FUNCTION__, ctx->renderer, sfc);
				}
				else{
					return scm_errorstrf("TTF_RenderGlyph_Shaded error: %s", TTF_GetError());
				}
			}
			else{
				return scm_errorstr("expected color");
			}
		}
		else{
			return scm_errorstr("invalid handle");
		}
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-render-text-blended"
static SCM font_render_text_blended(SCM scm_ctx, SCM font, SCM scm_text, SCM scm_color){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		font_handle *handle = scm_to_font(font);
		if(handle && handle->font){
			char *text = scm_to_locale_string(scm_text);
			SDL_Color color;
			if(scm_to_rgba(scm_color, &color)){
				SDL_Surface *sfc = TTF_RenderText_Blended(handle->font, text, color);
				if(sfc){
					free(text);
					return texture_from_surface(__SCM_FUNCTION__, ctx->renderer, sfc);
				}
				else{
					free(text);
					return scm_errorstrf("TTF_RenderText_Blended error: %s", TTF_GetError());
				}
			}
			else{
				free(text);
				return scm_errorstr("expected color");
			}
		}
		else{
			return scm_errorstr("invalid handle");
		}
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__

#define __SCM_FUNCTION__ "font-render-glyph-blended"
static SCM font_render_glyph_blended(SCM scm_ctx, SCM font, SCM scm_glyph, SCM scm_color){
	playctx *ctx = scm_to_playctx(scm_ctx);
	if(ctx){
		font_handle *handle = scm_to_font(font);
		if(handle && handle->font){
			uint16_t glyph = scm_to_uint16(scm_char_to_integer(scm_glyph));
			SDL_Color color;
			if(scm_to_rgba(scm_color, &color)){
				SDL_Surface *sfc = TTF_RenderGlyph_Blended(handle->font, glyph, color);
				if(sfc){
					return texture_from_surface(__SCM_FUNCTION__, ctx->renderer, sfc);
				}
				else{
					return scm_errorstrf("TTF_RenderGlyph_Blended error: %s", TTF_GetError());
				}
			}
			else{
				return scm_errorstr("expected color");
			}
		}
		else{
			return scm_errorstr("invalid handle");
		}
	}
	else{
		return scm_errorstr("invalid context");
	}
}
#undef __SCM_FUNCTION__


void init_ttf(){
	if(TTF_Init() == -1){
		fprintf(stderr, "TTF_Init error: %s", TTF_GetError());
		exit(-1);
	}

	scm_c_define_gsubr("open-font", 2, 0, 0, open_font);
	scm_c_export("open-font");

	scm_c_define_gsubr("font-bold-p", 1, 0, 0, font_bold_p);
	scm_c_export("font-bold-p");

	scm_c_define_gsubr("font-italic-p", 1, 0, 0, font_italic_p);
	scm_c_export("font-italic-p");

	scm_c_define_gsubr("font-underline-p", 1, 0, 0, font_underline_p);
	scm_c_export("font-underline-p");

	scm_c_define_gsubr("font-strikethrough-p", 1, 0, 0, font_strikethrough_p);
	scm_c_export("font-strikethrough-p");

	scm_c_define_gsubr("font-normal-p", 1, 0, 0, font_normal_p);
	scm_c_export("font-normal-p");

	scm_c_define_gsubr("font-bold", 1, 0, 0, font_bold);
	scm_c_export("font-bold");

	scm_c_define_gsubr("font-italic", 1, 0, 0, font_italic);
	scm_c_export("font-italic");

	scm_c_define_gsubr("font-underline", 1, 0, 0, font_underline);
	scm_c_export("font-underline");

	scm_c_define_gsubr("font-strikethrough", 1, 0, 0, font_strikethrough);
	scm_c_export("font-strikethrough");

	scm_c_define_gsubr("font-normal", 1, 0, 0, font_normal);
	scm_c_export("font-normal");

	scm_c_define_gsubr("font-ascent", 1, 0, 0, font_ascent);
	scm_c_export("font-ascent");

	scm_c_define_gsubr("font-descent", 1, 0, 0, font_descent);
	scm_c_export("font-descent");

	scm_c_define_gsubr("font-height", 1, 0, 0, font_height);
	scm_c_export("font-height");

	scm_c_define_gsubr("font-faces", 1, 0, 0, font_faces);
	scm_c_export("font-faces");

	scm_c_define_gsubr("font-lineskip", 1, 0, 0, font_lineskip);
	scm_c_export("font-lineskip");

	scm_c_define_gsubr("font-kerning", 2, 0, 0, font_kerning);
	scm_c_export("font-kerning");

	scm_c_define_gsubr("font-kerning-p", 1, 0, 0, font_kerning_p);
	scm_c_export("font-kerning-p");

	scm_c_define_gsubr("font-outline", 2, 0, 0, font_outline);
	scm_c_export("font-outline");

	scm_c_define_gsubr("font-family-name", 1, 0, 0, font_family_name);
	scm_c_export("font-family-name");

	scm_c_define_gsubr("font-style-name", 1, 0, 0, font_style_name);
	scm_c_export("font-style-name");

	scm_c_define_gsubr("font-fixed-width-p", 1, 0, 0, font_fixed_width_p);
	scm_c_export("font-fixed-width-p");

	scm_c_define_gsubr("font-provides-glyph-p", 2, 0, 0, font_provides_glyph_p);
	scm_c_export("font-provides-glyph-p");

	scm_c_define_gsubr("font-glyph-minx", 2, 0, 0, font_glyph_minx);
	scm_c_export("font-glyph-minx");

	scm_c_define_gsubr("font-glyph-maxx", 2, 0, 0, font_glyph_maxx);
	scm_c_export("font-glyph-maxx");

	scm_c_define_gsubr("font-glyph-miny", 2, 0, 0, font_glyph_miny);
	scm_c_export("font-glyph-miny");

	scm_c_define_gsubr("font-glyph-maxy", 2, 0, 0, font_glyph_maxy);
	scm_c_export("font-glyph-maxy");

	scm_c_define_gsubr("font-glyph-advance", 2, 0, 0, font_glyph_advance);
	scm_c_export("font-glyph-advance");

	scm_c_define_gsubr("font-size-text", 2, 0, 0, font_size_text);
	scm_c_export("font-size-text");

	scm_c_define_gsubr("font-render-text-solid", 4, 0, 0, font_render_text_solid);
	scm_c_export("font-render-text-solid");

	scm_c_define_gsubr("font-render-glyph-solid", 4, 0, 0, font_render_glyph_solid);
	scm_c_export("font-render-glyph-solid");

	scm_c_define_gsubr("font-render-text-shaded", 5, 0, 0, font_render_text_shaded);
	scm_c_export("font-render-text-shaded");

	scm_c_define_gsubr("font-render-glyph-shaded", 5, 0, 0, font_render_glyph_shaded);
	scm_c_export("font-render-glyph-shaded");

	scm_c_define_gsubr("font-render-text-blended", 4, 0, 0, font_render_text_blended);
	scm_c_export("font-render-text-blended");

	scm_c_define_gsubr("font-render-glyph-blended", 4, 0, 0, font_render_glyph_blended);
	scm_c_export("font-render-glyph-blended");
}
