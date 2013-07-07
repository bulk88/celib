// Sample config.h for GNU packages

#ifndef CONFIG_H
#define CONFIG_H

#define STDC_HEADERS 1
#define HAVE_LIMITS_H 1
#define HAVE_STRERROR 1
#define HAVE_VPRINTF 1
#define HAVE_UNISTD_H 1
#define HAVE_MEMCPY 1
#define HAVE_STRING_H 1
#define HAVE_STDC_HEADERS 1
#define HAVE_ALLOCA_H 1
#define HAVE_FCNTL_H 1
#define HAVE_NDIR_H 1

#ifdef UNDER_CE
#include "celib.h"
#endif

#endif
