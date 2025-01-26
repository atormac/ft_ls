#ifndef H_TREE
#define H_TREE

#include "ft_ls.h"

struct tree_node
{
	char *name;
	bool is_directory;
	int num_child;
	struct tree_node **children;
};

struct	tree_node *tree_create_node(const char *filename, bool is_dir);
void	tree_print(struct tree_node *node, int depth);

#endif
