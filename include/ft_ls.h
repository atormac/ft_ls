#ifndef H_FT_LS
#define H_FT_LS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>


enum
{
	LS_OK = 0,
	LS_MINOR = 1,
	LS_FATAL = 2,
};

enum
{
	F_HIDDEN = 1 << 1,
	F_LONG = 1 << 2,
	F_RECURSIVE = 1 << 3,
	F_SORTED = 1 << 4,
	F_REVERSE = 1 << 5,
};

extern int opt;

typedef struct t_entry
{
	long	blocks;
	time_t	mtime;
	mode_t	mode;
	nlink_t links;
	off_t	size;
    	uid_t	uid; //owner id
    	gid_t	gid; //group id

	bool	is_dir;	
	char	*filename;
	char	*fullpath;
}	t_entry;

typedef struct t_head
{
	int	count;
	char	*base_path; //probably not needed
	t_entry	*entries;
}	t_head;

#endif
