#ifndef RGBA_H
#define RGBA_H
#pragma once

#include <stdbool.h>
#include <libguile.h>
#include <SDL.h>

extern SCM scm_from_rgba(const SDL_Color *color);
extern bool scm_to_rgba(SCM scm_rgba, SDL_Color *color);
extern uint32_t rgba_to_uint32(const SDL_Color *color);
#endif