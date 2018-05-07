#ifndef GRAPHICS_H
#define GRAPHICS_H
#pragma once
#include <libguile.h>
#include <SDL.h>
typedef struct texture_handle texture_handle;

extern void init_graphics();
extern SCM texture_from_surface(const char *proc, SDL_Renderer *renderer, SDL_Surface *surface);
extern SCM scm_from_tex(texture_handle *tex);
extern texture_handle *scm_to_tex(SCM tex);
#endif