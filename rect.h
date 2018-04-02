#ifndef RECT_H
#define RECT_H
#pragma once
#include <stdbool.h>
#include <SDL.h>
#include <libguile.h>

bool scm_to_rect(SCM scm_rect, SDL_Rect *rect);
SCM scm_from_rect(SDL_Rect *rect);
#endif