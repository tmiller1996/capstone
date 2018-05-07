#ifndef POINT_H
#define POINT_H
#pragma once
#include <stdbool.h>
#include <SDL.h>
#include <libguile.h>
extern bool scm_to_point(SCM scm_point, SDL_Point *point);
extern SCM scm_from_point(SDL_Point *point);
#endif