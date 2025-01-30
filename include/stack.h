#ifndef H_STACK
#define H_STACK

#include <stdlib.h>

typedef struct t_node
{
	char *path;
	struct t_node *next;
}	t_node;

void	stack_push(t_node **top, const char *path);
char	*stack_pop(t_node **top);

#endif
