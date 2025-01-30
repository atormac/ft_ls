#include "../include/stack.h"
#include "../include/str.h"

void stack_push(t_node **top, const char *path)
{
	t_node *new = malloc(sizeof(t_node));
	if (new == NULL)
		return;

	new->path = ft_strdup(path);
	new->next = *top;
	*top = new;
}

char *stack_pop(t_node **top)
{
	if (*top == NULL)
		return NULL;

	t_node *temp = *top;
	char *path = temp->path;
	*top = temp->next;
	free(temp);
	return path;
}
