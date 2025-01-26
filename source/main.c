#include "../include/ft_ls.h"
#include "../include/tree.h"
#include <errno.h>

char*	g_path = ".";
int	opt = 0;

void error_dir(char *dir)
{
	char buf[1024];

	snprintf(buf, sizeof(buf), "Error: cannot open directory: '%s': %s\n",
			dir, strerror(errno));
	write(STDERR_FILENO, buf, strlen(buf));
}

bool list_dir(char *path, struct tree_node *parent)
{
	DIR		*dir;
	struct dirent	*entry;

	if (!(dir = opendir(path))) {
		error_dir(path);
		return false;
	}

	while ((entry = readdir(dir)) != NULL) {

		if (!(opt & F_HIDDEN) && entry->d_name[0] == '.')
			continue;

		char full_path[1024];
        	snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
	
		struct stat st;
		if (stat(full_path, &st) != 0)
			continue;

		bool should_recurse = S_ISDIR(st.st_mode);
		if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
			should_recurse = false;

		struct tree_node *new_child = tree_create_node(entry->d_name, S_ISDIR(st.st_mode));
        	parent->children = realloc(parent->children, (parent->num_child + 1) * sizeof(struct tree_node *));
        	parent->children[parent->num_child++] = new_child;

		if ((opt & F_RECURSIVE) && should_recurse) {
			list_dir(full_path, parent);
		}
	}
	closedir(dir);
	return true;
}

bool parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc && argv[i][0] == '-'; i++) {
		
		switch(argv[i][1]) {
			case	'a': opt |= F_HIDDEN; break;
			case	'l': opt |= F_LONG; break;
			case	'R': opt |= F_RECURSIVE; break;
			case	't': opt |= F_SORTED; break;
			case	'r': opt |= F_REVERSE; break;
			default:
				     printf("Error: unknown option: %s\n", argv[i]);
				     return false;
		}
	}
	if (i < argc)
		g_path = argv[i];
	printf("PATH: %s\n", g_path);
	if (opt & F_HIDDEN) printf("[opt]: F_HIDDEN\n");
	if (opt & F_RECURSIVE) printf("[opt]: F_RECURSIVE\n");
	if (opt & F_SORTED) printf("[opt]: F_SORTED\n");
	if (opt & F_REVERSE) printf("[opt]: F_REVERSE\n");
	printf("------------------------------------\n");

	return true;
}

int main(int argc, char **argv)
{
	if (!parse_args(argc, argv))
		return EXIT_FAILURE;

	struct tree_node *root = tree_create_node(g_path, 1);
	bool ret = list_dir(g_path, root);

	tree_print(root, 0);
	return !ret;
}
