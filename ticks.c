#include "ticks.h"
#include <libguile.h>
#include <SDL.h>

static SCM get_ticks(){
	return scm_from_uint32(SDL_GetTicks());
}

void init_ticks(){
	scm_c_define_gsubr("get-ticks", 0, 0, 0, get_ticks);
	scm_c_export("get-ticks");
}