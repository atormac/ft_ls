#include "../include/ft_ls.h"
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

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

int len_digits(long num)
{
	int len = 1;
	while (num >= 10) {

		len++;
		num /= 10;
	}
	return len;	
}

void format_user(char *s, int max_size)
{
	int len = strlen(s);
	int pad = max_size - len;

	write(1, s, len);
	do {
		write(1, " ", 1);
	} while (pad--);
}

void print_ulong(unsigned long n)
{
	if (n >= 10)
		print_ulong(n / 10);
	char ch = '0' + (n % 10);
	write(1, &ch, 1);
}

void format_ulong(unsigned long n, int max_size)
{
	int pad = max_size - len_digits(n);
	while (pad > 0) {
		write(1, " ", 1);
		pad--;
	}
	print_ulong(n);
}

void print_longmode(t_head *head)
{
	unsigned long max_link_size = 0;
	long max_size = 0;
	int max_owner_len = 0;
	int max_group_len = 0;
	long total_blocks = 0;

	for (int i = 0; i < head->count; i++) {
		t_entry *entry = &head->entries[i];

		total_blocks += entry->blocks;
		if (entry->links > max_link_size)
			max_link_size = entry->links;
		if (entry->size > max_size)
			max_size = entry->size;
		struct passwd *pwd = getpwuid(entry->gid);
		struct group *grp = getgrgid(entry->gid);
		char *owner = pwd ? pwd->pw_name : "unknown";
		char *group = grp ? grp->gr_name : "unknown";
		int owner_len = strlen(owner);
		int group_len = strlen(group);
		if (owner_len > max_owner_len)
			max_owner_len = owner_len;
		if (group_len > max_group_len)
			max_group_len = group_len;
	}
	max_link_size = len_digits(max_link_size);
	int max_size_len = len_digits(max_size);

	printf("total: %ld\n", total_blocks / 2);

	for (int i = 0; i < head->count; i++) {
		t_entry *entry = &head->entries[i];
	
		print_permissions(entry->mode);
		//printf("%lu ", entry->links);

		format_ulong(entry->links, max_link_size);
		write(1, " ", 1);
		//printf("%*lu ", (int)max_link_size, entry->links);

		struct passwd *pwd = getpwuid(entry->gid);
		struct group *grp = getgrgid(entry->gid);
		format_user(pwd->pw_name, max_owner_len);
		format_user(grp->gr_name, max_group_len);
		//printf("%-*s ", max_owner_len, pwd ? pwd->pw_name : "unknown");
		//printf("%-*s ", max_group_len, grp ? grp->gr_name : "unknown");


		format_ulong(entry->size, max_size_len);
		//printf("%*ld", max_size_len, (long)entry->size);
	
		char buffer[64];
		struct tm *tm_info = localtime(&entry->mtime);
		strftime(buffer, sizeof(buffer), "%b %d %H:%M", tm_info);
		//char *time_str = ctime(&entry->mtime);
		printf(" %s", buffer);
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
	printf("%s ", perms);
}
