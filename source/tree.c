#include "../include/tree.h"

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
