#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

enum {
	F_HIDDEN = 1 << 1,
	F_RECURSIVE = 1 << 2,
	F_SORTED = 1 << 3,
	F_REVERSE = 1 << 4,
};

char*	path = ".";
int	opt = 0;

bool list_dir(char *path)
{
	DIR		*dir;
	struct dirent	*entry;

	if (!(dir = opendir(path)))
		return false;

	while ((entry = readdir(dir)) != NULL) {
		printf("%s\n", entry->d_name);
	}

	closedir(dir);
	return true;
}

bool parse_flags(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc && argv[i][0] == '-'; i++) {
		
		switch(argv[i][1]) {
			case	'a': opt |= F_HIDDEN; break;
			case	'R': opt |= F_RECURSIVE; break;
			case	't': opt |= F_SORTED; break;
			case	'r': opt |= F_REVERSE; break;
			default: printf("Error: unknown option: %s\n", argv[i]); return false;
		}
	}
	if (i < argc)
		path = argv[i];
	printf("PATH: %s\n", path);
	if (opt & F_HIDDEN) printf("[opt]: F_HIDDEN\n");
	if (opt & F_RECURSIVE) printf("[opt]: F_RECURSIVE\n");
	if (opt & F_SORTED) printf("[opt]: F_SORTED\n");
	if (opt & F_REVERSE) printf("[opt]: F_REVERSE\n");

	return true;
}

int main(int argc, char **argv)
{
	if (!parse_flags(argc, argv))
		return EXIT_FAILURE;

	return list_dir(path);
}
