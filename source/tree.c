#include "../include/tree.h"

struct tree_node *tree_create_node(const char *filename, bool is_dir)
{
	struct tree_node *node = (struct tree_node *)malloc(sizeof(struct tree_node));
	node->name = strdup(filename);
	node->is_directory = is_dir;
	node->children = NULL;
	node->num_child = 0;
	return node;
}

void tree_print(struct tree_node *node, int depth) {

	/* for (int i = 1; i < depth; i++) {
		printf("  ");
	} */
	//if (depth > 0)
	if ((opt & F_RECURSIVE) && depth == 0) {
		printf("%s\n", node->name);
	}
	if (depth > 0) {
		if (opt & F_RECURSIVE && node->is_directory && node->num_child) {
			printf("%s:\n", node->name);
		}
		else {
			printf("%s\n", node->name);
		}
	}
	for (int i = 0; i < node->num_child; i++) {
		tree_print(node->children[i], depth + 1);
	}
}
