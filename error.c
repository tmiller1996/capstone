#include "error.h"

#include <stdio.h>
#include <stdarg.h>
#include <libguile.h>

SCM __scm_errorstr(const char *subr, const char *message){
	SCM scm_message = scm_from_locale_string(message);
	SCM scm_subr = scm_from_locale_string(subr);
	return scm_error_scm(SCM_BOOL_F, scm_subr, scm_message, SCM_BOOL_F, SCM_BOOL_F);
}

SCM __scm_errorstrf(const char *subr, const char *fmt, ...){
	va_list args;
	char buffer[256];
	va_start(args, fmt);
	vsnprintf(buffer, 256, fmt, args);
	return __scm_errorstr(subr, buffer);
}