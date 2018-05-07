#ifndef ERROR_H
#define ERROR_H
#pragma once
#include <libguile.h>
#ifndef scm_errorstr
extern SCM __scm_errorstr(const char *subr, const char *message);
#define scm_errorstr(message) (__scm_errorstr(__SCM_FUNCTION__, message))
#endif
#ifndef scm_errorstrf
extern SCM __scm_errorstrf(const char *subr, const char *fmt, ...);
#define scm_errorstrf(fmt, ...) __scm_errorstrf(__SCM_FUNCTION__, fmt, __VA_ARGS__);
#endif
#endif