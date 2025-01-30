#include "../include/ft_ls.h"
#include <errno.h>

void set_exit_status(int status);
void print_dir(t_head *head);
void free_list(t_head *head);

void error_dir(char *dir)
{
	char buf[1024];

	snprintf(buf, sizeof(buf), "Error: cannot open directory: '%s': %s\n",
			dir, strerror(errno));
	write(STDERR_FILENO, buf, strlen(buf));
}

int cmp_entry(const void *a, const void *b)
{
	struct t_entry *entry_a = (struct t_entry *)a;
	struct t_entry *entry_b = (struct t_entry *)b;
	int res = 0;

	if (opt & F_SORTED) {
		if (entry_a->mtime > entry_b->mtime) res =  -1;
    		if (entry_a->mtime < entry_b->mtime) res = 1;
	}
	else {
		//res = strcmp(entry_a->filename, entry_b->filename);
		res = strcoll(entry_a->filename, entry_b->filename);
	}

	if (opt & F_REVERSE)
		res *= -1;
	return res;
}


bool ls_add_entry(t_head *head, struct dirent *e, char *path)
{
	char full_path[1024];
	struct stat st;

	snprintf(full_path, sizeof(full_path), "%s/%s", path, e->d_name);
	if (lstat(full_path, &st) != 0)
		return false;

	if (head->alloc_count - head->count == 0) {
		int new_size = head->count + 8;
		head->entries = realloc(head->entries, new_size * sizeof(t_entry));
		head->alloc_count = new_size;
	}
	t_entry *entry = &head->entries[head->count];

	entry->blocks = st.st_blocks;
	entry->mtime = st.st_mtime;
	entry->mode = st.st_mode;
	entry->links = st.st_nlink;
	entry->size = st.st_size;
	entry->uid = st.st_uid;
	entry->gid = st.st_gid;
	entry->filename = strdup(e->d_name);
	entry->fullpath = NULL;

	if (opt & F_RECURSIVE && S_ISDIR(st.st_mode))
		entry->fullpath = strdup(full_path);
	else if ((opt & F_LONG) && (entry->mode & S_IFMT) == S_IFLNK)
		entry->fullpath = strdup(full_path);
	head->count++;
	return true;
}

bool ls_dir(char *path)
{
	DIR		*dir;
	struct dirent	*e;
	struct t_head head = { 0 };

	if (!(dir = opendir(path))) {
		set_exit_status(LS_MINOR);
		error_dir(path);
		return false;
	}
	head.base_path = strdup(path);

	while ((e = readdir(dir)) != NULL) {

		if (!(opt & F_HIDDEN) && e->d_name[0] == '.')
			continue;
		ls_add_entry(&head, e, path);
	}
	closedir(dir);

	if (head.entries)
		qsort(head.entries, head.count, sizeof(t_entry), cmp_entry);
	print_dir(&head);

	if (!(opt & F_RECURSIVE)) {
		free_list(&head);
		return true;
	}
	for (int i = 0; i < head.count; i++) {

		if (!S_ISDIR(head.entries[i].mode))
			continue;
		if ((strcmp(head.entries[i].filename, ".") == 0) || (strcmp(head.entries[i].filename, "..") == 0))
			continue;

		ls_dir(head.entries[i].fullpath);
	}

	free_list(&head);
	return true;
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
