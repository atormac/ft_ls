#include "../include/ft_ls.h"
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

void print_default(t_head *head);
void print_longmode(t_head *head);

void print_dir(t_head *head)
{
	static bool has_printed;

	if ((opt & F_RECURSIVE)) {
		if (has_printed)
			printf("\n");
		printf("%s:\n", head->base_path);
		has_printed = true;
	}
	if (opt & F_LONG)
		print_longmode(head);
	else
		print_default(head);

}

void print_default(t_head *head)
{
	int i;
	for (i = 0; i < head->count - 1; i++) {
		printf("%s  ", head->entries[i].filename);
	}
	if (i < head->count) {
		printf("%s\n", head->entries[i].filename);
	}
}

void print_permissions(mode_t mode);

void print_longmode(t_head *head)
{
	for (int i = 0; i < head->count; i++) {
		t_entry *entry = &head->entries[i];
	
		print_permissions(entry->mode);
		printf(" %lu", entry->links);

		struct passwd *pwd = getpwuid(entry->gid);
		struct group *grp = getgrgid(entry->gid);
		printf(" %s", pwd ? pwd->pw_name : "unknown");
		printf(" %s", grp ? grp->gr_name : "unknown");

		printf(" %ld", entry->size);
		printf(" %s\n", entry->filename);
	}
}

void print_permissions(mode_t mode)
{
	char perms[11] = "----------";

	if (S_ISDIR(mode)) perms[0] = 'd';
	else if (S_ISCHR(mode)) perms[0] = 'c';
	else if (S_ISBLK(mode)) perms[0] = 'b';
	else if (S_ISLNK(mode)) perms[0] = 'l';
	else if (S_ISFIFO(mode)) perms[0] = 'p';
	else if (S_ISSOCK(mode)) perms[0] = 's';

	//owner
	perms[1] = (mode & S_IRUSR) ? 'r' : '-';
	perms[2] = (mode & S_IWUSR) ? 'w' : '-';
	perms[3] = (mode & S_IXUSR) ? 'x' : '-';

	//group
	perms[4] = (mode & S_IRGRP) ? 'r' : '-';
	perms[5] = (mode & S_IWGRP) ? 'w' : '-';
	perms[6] = (mode & S_IXGRP) ? 'x' : '-';

	//other
	perms[7] = (mode & S_IROTH) ? 'r' : '-';
	perms[8] = (mode & S_IWOTH) ? 'w' : '-';
	perms[9] = (mode & S_IXOTH) ? 'x' : '-';

	// setuid, setgid, sticky bit
	if (mode & S_ISUID) perms[3] = (perms[3] == 'x') ? 's' : 'S';
	if (mode & S_ISGID) perms[6] = (perms[6] == 'x') ? 's' : 'S';
	if (mode & S_ISVTX) perms[9] = (perms[9] == 'x') ? 't' : 'T';

	perms[10] = '\0';
	printf("%s", perms);
}
