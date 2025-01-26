#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

enum {
	F_HIDDEN = 1 << 1,
	F_LONG = 1 << 2,
	F_RECURSIVE = 1 << 3,
	F_SORTED = 1 << 4,
	F_REVERSE = 1 << 5,
};

char*	path = ".";
int	opt = 0;

struct tree_node
{
	char *name;
	bool is_directory;
	int num_child;
	struct tree_node **children;
};

struct tree_node *tree_create_node(const char *filename, bool is_dir)
{
	struct tree_node *node = (struct tree_node *)malloc(sizeof(struct tree_node));
	node->name = filename ? strdup(filename) : NULL;
	node->is_directory = is_dir;
	node->children = NULL;
	node->num_child = 0;
	return node;
}

void tree_print(struct tree_node *node, int depth) {

	for (int i = 1; i < depth; i++) {
		printf("  ");
	}
	if (depth > 0)
		printf("%s\n", node->name);
	for (int i = 0; i < node->num_child; i++) {
		tree_print(node->children[i], depth + 1);
	}
}

bool list_dir(char *path)
{
	DIR		*dir;
	struct dirent	*entry;

	if (!(dir = opendir(path)))
		return false;
	struct tree_node *parent = tree_create_node(path, 1);

	while ((entry = readdir(dir)) != NULL) {

		if (!(opt & F_HIDDEN) && entry->d_name[0] == '.')
			continue;

		struct stat st;
		if (stat(entry->d_name, &st) != 0)
			continue;

		struct tree_node *new_child = tree_create_node(entry->d_name, S_ISDIR(st.st_mode));
        	parent->children = realloc(parent->children, (parent->num_child + 1) * sizeof(struct tree_node *));
        	parent->children[parent->num_child++] = new_child;

	}
	closedir(dir);

	tree_print(parent, 0);
	return true;
}

bool parse_flags(int argc, char **argv)
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
		path = argv[i];
	printf("PATH: %s\n", path);
	if (opt & F_HIDDEN) printf("[opt]: F_HIDDEN\n");
	if (opt & F_RECURSIVE) printf("[opt]: F_RECURSIVE\n");
	if (opt & F_SORTED) printf("[opt]: F_SORTED\n");
	if (opt & F_REVERSE) printf("[opt]: F_REVERSE\n");
	printf("------------------------------------\n");

	return true;
}

int main(int argc, char **argv)
{
	if (!parse_flags(argc, argv))
		return EXIT_FAILURE;

	return list_dir(path);
}
