#include "rect.h"

bool scm_to_rect(SCM scm_rect, SDL_Rect *rect){
	if(rect){
		SCM scm_len = scm_length(scm_rect);
		int len = scm_to_int(scm_len);
		if(len == 4){
			SCM scm_x = scm_car(scm_rect);
			SCM scm_y = scm_car(scm_cdr(scm_rect));
			SCM scm_w = scm_car(scm_cdr(scm_cdr(scm_rect)));
			SCM scm_h = scm_car(scm_cdr(scm_cdr(scm_cdr(scm_rect))));
			rect->x = scm_to_int(scm_x);
			rect->y = scm_to_int(scm_y);
			rect->w = scm_to_int(scm_w);
			rect->h = scm_to_int(scm_h);
			return true;
		}
	}
	return false;
}

SCM scm_from_rect(SDL_Rect *rect){
	return scm_list_4(
		scm_from_int(rect->x), 
		scm_from_int(rect->y), 
		scm_from_int(rect->w), 
		scm_from_int(rect->h)
		);
}
