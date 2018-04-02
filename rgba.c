#include "rgba.h"

SCM scm_from_rgba(struct rgba rgba){
	return scm_list_4(
		scm_from_uint8(rgba.r), scm_from_uint8(rgba.g),
		scm_from_uint8(rgba.b), scm_from_uint8(rgba.a)
		);
}

struct rgba scm_to_rgba(SCM scm_rgba){
	struct rgba rgba;
	SCM scm_r = scm_car(scm_rgba);
	SCM scm_g = scm_car(scm_cdr(scm_rgba));
	SCM scm_b = scm_car(scm_cdr(scm_cdr(scm_rgba)));
	SCM scm_a = scm_car(scm_cdr(scm_cdr(scm_cdr(scm_rgba))));
	rgba.r = scm_to_uint8(scm_r);
	rgba.g = scm_to_uint8(scm_g);
	rgba.b = scm_to_uint8(scm_b);
	rgba.a = scm_to_uint8(scm_a);
	return rgba;
}