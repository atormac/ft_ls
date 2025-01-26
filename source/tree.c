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

void tree_print_node(struct tree_node *root)
{
	int i;
	for (i = 0; i < root->num_child - 1; i++) {
		struct tree_node *node = root->children[i];
		printf("%s  ", node->name);
	}
	if (i < root->num_child) {
		struct tree_node *node = root->children[i];
		printf("%s\n", node->name);
	}
}

void tree_print(struct tree_node *node, int depth) {

	if (!(opt & F_RECURSIVE)) {
		tree_print_node(node);
		return;
	}
	//tree_print_recursive(node, 0);
		
	if (node->is_directory)
	{
		printf("%s:\n", node->name);
		tree_print_node(node);
		printf("\n");
	}
	for (int i = 0; i < node->num_child; i++) {
		tree_print(node->children[i], depth + 1);
	}
		
}
