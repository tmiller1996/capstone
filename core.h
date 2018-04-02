#ifndef CORE_H
#define CORE_H
#pragma once

#include <SDL.h>
#include <stdbool.h>

struct play_context {
	struct {
		uint8_t *last;
		uint8_t *current;
		int numkeys;
	} keyboard;
	struct {
		int x, y;
		uint32_t last;
		uint32_t current;
	} mouse;
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool close_requested;
};

typedef struct play_context playctx;

#endif