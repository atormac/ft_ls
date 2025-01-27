#include "../include/ft_ls.h"

void print_dir(t_head *head)
{
	static bool has_printed;
	int i;

	if ((opt & F_RECURSIVE)) {
		if (has_printed)
			printf("\n");
		printf("%s:\n", head->base_path);
		has_printed = true;
	}

	for (i = 0; i < head->count - 1; i++) {
		printf("%s  ", head->entries[i].filename);
	}
	if (i < head->count) {
		printf("%s\n", head->entries[i].filename);
	}
}
