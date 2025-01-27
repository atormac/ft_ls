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

typedef struct t_entry
{
	time_t	mtime;
	bool	is_dir;	
	char	*filename;
	char	*fullpath;
}	t_entry;

typedef struct t_head
{
	int	count;
	bool	has_printed;
	char	*base_path;
	t_entry	*entries;
}	t_head;


int cmp_entry(const void *a, const void *b)
{
	int res = 0;
	struct t_entry *entry_a = (struct t_entry *)a;
	struct t_entry *entry_b = (struct t_entry *)b;

	if (opt & F_SORTED) {
		if (entry_a->mtime > entry_b->mtime) res =  -1;
    		if (entry_a->mtime < entry_b->mtime) res = 1;
	}
	else {
		res = strcmp(entry_a->filename, entry_b->filename);
	}

	if (opt & F_REVERSE)
		res *= -1;
	return res;
}

void print_dir_entries(t_head *head)
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

void free_list(t_head *head)
{
	if (!head)
		return;
	for (int i = 0; i < head->count; i++) {
		free(head->entries[i].filename);
		free(head->entries[i].fullpath);
	}
	free(head->base_path);
	free(head->entries);
}

bool list_dir(char *path)
{
	DIR		*dir;
	struct dirent	*e;

	if (!(dir = opendir(path))) {
		error_dir(path);
		return false;
	}
	struct t_head head = { 0 };
	head.base_path = strdup(path);

	while ((e = readdir(dir)) != NULL) {

		if (!(opt & F_HIDDEN) && e->d_name[0] == '.')
			continue;

		char full_path[1024];
		struct stat st;
        	snprintf(full_path, sizeof(full_path), "%s/%s", path, e->d_name);
		if (stat(full_path, &st) != 0)
			continue;

		head.entries = realloc(head.entries, (head.count + 1) * sizeof(t_entry));
		head.entries[head.count].mtime = st.st_mtime;
		head.entries[head.count].is_dir = S_ISDIR(st.st_mode);
		head.entries[head.count].filename = strdup(e->d_name);
		head.entries[head.count].fullpath = NULL;
		if (opt & F_RECURSIVE && S_ISDIR(st.st_mode))
			head.entries[head.count].fullpath = strdup(full_path);
		head.count++;
	}
	closedir(dir);
	if (!head.entries || head.count == 0) {
		free_list(&head);
		return false;
	}
	qsort(head.entries, head.count, sizeof(t_entry), cmp_entry);
	print_dir_entries(&head);

	if (!(opt & F_RECURSIVE)) {
		free_list(&head);
		return true;
	}

	for (int i = 0; i < head.count; i++) {

		if (!head.entries[i].is_dir)
			continue;
		if ((strcmp(head.entries[i].filename, ".") == 0) || (strcmp(head.entries[i].filename, "..") == 0))
			continue;

		list_dir(head.entries[i].fullpath);
	}

	free_list(&head);
	return true;
}

bool parse_args(int argc, char **argv)
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
		g_path = argv[i];
	printf("PATH: %s\n", g_path);
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
	if (!parse_args(argc, argv))
		return EXIT_FAILURE;

	bool ret = list_dir(g_path);
	return !ret;
}
