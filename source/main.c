#include "../include/ft_ls.h"
#include "../include/tree.h"
#include <locale.h>

int	opt = 0;

static int g_exit_status = LS_OK;

bool ls_dir(char *path);

void set_exit_status(int status)
{
	g_exit_status = status;
}

static int cmp_dirs(const void *a, const void *b)
{
	const char *dir_a = *(const char**)a;
	const char *dir_b = *(const char**)b;

	return strcmp(dir_a, dir_b);
}

char	**dirs = NULL;
int	dir_count = 0;

static bool add_dirs(int argc, char **argv)
{
	dirs = malloc(64 * sizeof(char *));
	if (!dirs)
		return false;

	for (int i = 1; i < argc; i++) {

		if (argv[i][0] == '-' && argv[i][1])
			continue;
		dirs[dir_count] = strdup(argv[i]);
		dir_count++;
	}
	if (!dir_count) {
		dirs[0] = strdup(".");
		dir_count = 1;
	}
	qsort(dirs, dir_count, sizeof(char *), cmp_dirs);
	for (int k = 0; k < dir_count; k++) {
		printf("%d: %s\n", k, dirs[k]);
	}
	return true;
}

static bool parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++) {
		for (int j = 1; argv[i][0] == '-' && argv[i][j]; j++) {
			switch(argv[i][j]) {
				case	'a': opt |= F_HIDDEN; break;
				case	'l': opt |= F_LONG; break;
				case	'R': opt |= F_RECURSIVE; break;
				case	't': opt |= F_SORTED; break;
				case	'r': opt |= F_REVERSE; break;
				default:
					     printf("ft_ls: invalid option: -- '%c'\n", argv[i][j]);
					     return false;
			}
		}
	}
	add_dirs(argc, argv);

	if (opt & F_HIDDEN) printf("[opt]: F_HIDDEN\n");
	if (opt & F_LONG) printf("[opt]: F_LONG\n");
	if (opt & F_RECURSIVE) printf("[opt]: F_RECURSIVE\n");
	if (opt & F_SORTED) printf("[opt]: F_SORTED\n");
	if (opt & F_REVERSE) printf("[opt]: F_REVERSE\n");
	printf("------------------------------------\n");

	return true;
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, ""); //debug
	setvbuf(stdout, NULL, _IONBF, 0);

	if (!parse_args(argc, argv))
		return EXIT_FAILURE;

	for (int i = 0; i < dir_count; i++) {
		ls_dir(dirs[i]);
		free(dirs[i]);
	}
	free(dirs);
	return g_exit_status;
}
