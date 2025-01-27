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

bool parse_args(int argc, char **argv, char **path)
{
	int i;
	for (i = 1; i < argc && argv[i][0] == '-' && argv[i][1]; i++) {
		for (int j = 1; argv[i][j]; j++) {
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
	if (i < argc)
		*path = argv[i];
	printf("PATH: %s\n", *path);
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
	char	*path = ".";

	if (!parse_args(argc, argv, &path))
		return EXIT_FAILURE;
	setvbuf(stdout, NULL, _IONBF, 0);
	setlocale(LC_ALL, ""); //debug

	ls_dir(path);
	return g_exit_status;
}
