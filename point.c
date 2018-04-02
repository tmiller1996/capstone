#include "point.h"

bool scm_to_point(SCM scm_point, SDL_Point *point){
	if(point){
		if(scm_is_pair(scm_point)){
			SCM scm_x = scm_car(scm_point);
			SCM scm_y = scm_cdr(scm_point);
			point->x = scm_to_int(scm_x);
			point->y = scm_to_int(scm_y);
			return true;
		}
	}
	return false;
}

SCM scm_from_point(SDL_Point *point){
	return scm_cons(
		scm_from_int(point->x),
		scm_from_int(point->y));
}