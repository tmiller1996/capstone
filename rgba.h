#ifndef RGBA_H
#define RGBA_H
#pragma once

#include <stdbool.h>
#include <libguile.h>

struct rgba {
	uint8_t r, g, b, a;
};

SCM scm_from_rgba(struct rgba *rgba);
bool scm_to_rgba(SCM scm_rgba, struct rgba *rgba);
#endif