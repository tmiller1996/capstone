#ifndef RGBA_H
#define RGBA_H
#pragma once

#include <stdbool.h>
#include <libguile.h>
#include <SDL.h>

SCM scm_from_rgba(SDL_Color *color);
bool scm_to_rgba(SCM scm_rgba, SDL_Color *color);
#endif