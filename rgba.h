#ifndef RGBA_H
#define RGBA_H
#pragma once

#include <libguile.h>

struct rgba {
	uint8_t r, g, b, a;
};

SCM scm_from_rgba(struct rgba rgba);
struct rgba scm_to_rgba(SCM scm_rgba);
#endif