#include "rgba.h"

SCM scm_from_rgba(const SDL_Color *color){
	return scm_list_4(
		scm_from_uint8(color->r), scm_from_uint8(color->g),
		scm_from_uint8(color->b), scm_from_uint8(color->a)
		);
}

bool scm_to_rgba(SCM scm_rgba, SDL_Color *color){
	if(color){
		SCM scm_len = scm_length(scm_rgba);
		int len = scm_to_int(scm_len);
		if(len == 4){
			SCM scm_r = scm_car(scm_rgba);
			SCM scm_g = scm_car(scm_cdr(scm_rgba));
			SCM scm_b = scm_car(scm_cdr(scm_cdr(scm_rgba)));
			SCM scm_a = scm_car(scm_cdr(scm_cdr(scm_cdr(scm_rgba))));
			color->r = scm_to_uint8(scm_r);
			color->g = scm_to_uint8(scm_g);
			color->b = scm_to_uint8(scm_b);
			color->a = scm_to_uint8(scm_a);
			return true;
		}
	}
	return false;
}

uint32_t rgba_to_uint32(const SDL_Color *color){
	uint32_t u;
	u = color->r;
	u = (u  << 8) + color->g;
	u = (u  << 8) + color->b;
	u = (u  << 8) + color->a;
	return u;
}