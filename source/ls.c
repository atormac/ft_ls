#include "../include/ft_ls.h"
#include <errno.h>

void set_exit_status(int status);
void print_dir(t_head *head);

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

bool ls_dir(char *path)
{
	DIR		*dir;
	struct dirent	*e;

	if (!(dir = opendir(path))) {
		set_exit_status(LS_MINOR);
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
		if (lstat(full_path, &st) != 0)
			continue;

		head.entries = realloc(head.entries, (head.count + 1) * sizeof(t_entry));
		head.entries[head.count].blocks = st.st_blocks;
		head.entries[head.count].mtime = st.st_mtime;
		head.entries[head.count].mode = st.st_mode;
		head.entries[head.count].links = st.st_nlink;
		head.entries[head.count].size = st.st_size;
		head.entries[head.count].uid = st.st_uid;
		head.entries[head.count].gid = st.st_gid;

		head.entries[head.count].filename = strdup(e->d_name);
		head.entries[head.count].fullpath = NULL;
		if (opt & F_RECURSIVE && S_ISDIR(st.st_mode))
			head.entries[head.count].fullpath = strdup(full_path);
		head.count++;
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
