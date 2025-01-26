#ifndef H_FT_LS
#define H_FT_LS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>



enum {
	F_HIDDEN = 1 << 1,
	F_LONG = 1 << 2,
	F_RECURSIVE = 1 << 3,
	F_SORTED = 1 << 4,
	F_REVERSE = 1 << 5,
};

#endif
