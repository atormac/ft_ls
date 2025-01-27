#include "../include/ft_ls.h"
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_default(t_head *head);
void print_longmode(t_head *head);
void put_chars(char *s);

void print_dir(t_head *head)
{
	static bool has_printed;

	if ((opt & F_RECURSIVE) || (dir_count >= 2)) {
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

void format_file(char *s, int max_size)
{
	int len = strlen(s);
	int pad = max_size - len;


	write(1, s, len);
	do {
		write(1, " ", 1);
	} while (pad-- > 0);
}

void print_default(t_head *head)
{
	int max_len = 0;

	for (int i = 0; i < head->count; i++) {
		int len = strlen(head->entries[i].filename);
		if (len > max_len) max_len = len;
	}
	if (!max_len)
		return;
	int col_width = max_len + 2;
	int cols =  100 / col_width;
	if (cols == 0) cols = 1;

	for (int i = 0; i < head->count; i++) {
		format_file(head->entries[i].filename, max_len);
		if ((i + 1) % cols == 0 || i == cols - 1 || i == head->count - 1) {
			put_chars("\n");
		}
	}
	/*
	int rows = (head->count + cols - 1) / cols;
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			int index = row + col * rows;
			if (index >= head->count)
				break;
			if (col == cols - 1) {
				put_chars(head->entries[index].filename);
				break;
			}
			format_file(head->entries[index].filename, max_len);
		}
		put_chars("\n");
	}
	*/
	/*
	int i;
	for (i = 0; i < head->count - 1; i++) {
		printf("%s  ", head->entries[i].filename);
	}
	if (i < head->count) {
		printf("%s\n", head->entries[i].filename);
	}
	*/
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
	} while (pad-- > 0);
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

void put_chars(char *s)
{
	size_t len = strlen(s);
	write(STDOUT_FILENO, s, len);
}

void put_line(char *s)
{
	put_chars(s);
	put_chars("\n");
}

void print_str(char *s, int len)
{
	if (!s)
		return;
	write(1, s, len);
}

void format_time(time_t mtime)
{
	char *time_str = ctime(&mtime);
	if (!time_str)
		return;
	write(1, " ", 1);
	print_str(&time_str[4], 12);
	write(1, " ", 1);
}

void format_symlink(t_entry *entry);

void print_longmode(t_head *head)
{
	unsigned long max_link_len = 0;
	long max_size_len = 0;
	int max_owner_len = 0;
	int max_group_len = 0;
	long total_blocks = 0;

	for (int i = 0; i < head->count; i++) {
		t_entry *entry = &head->entries[i];

		total_blocks += entry->blocks;
		if (entry->links > max_link_len)
			max_link_len = entry->links;
		if (entry->size > max_size_len)
			max_size_len = entry->size;

		struct passwd *pwd = getpwuid(entry->uid);
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
	max_link_len = len_digits(max_link_len);
	max_size_len = len_digits(max_size_len);

	put_chars("total: ");
	print_ulong(total_blocks / 2);
	put_chars("\n");

	for (int i = 0; i < head->count; i++) {
		t_entry *entry = &head->entries[i];

		print_permissions(entry->mode);

		format_ulong(entry->links, max_link_len);
		put_chars(" ");

		struct passwd *pwd = getpwuid(entry->uid);
		struct group *grp = getgrgid(entry->gid);
		char *owner = pwd ? pwd->pw_name : "unknown";
		char *group = grp ? grp->gr_name : "unknown";
		format_user(owner, max_owner_len);
		format_user(group, max_group_len);

		format_ulong(entry->size, max_size_len);
		format_time(entry->mtime);
		put_chars(entry->filename);
		format_symlink(entry);
		put_chars("\n");
	}
}

void format_symlink(t_entry *entry)
{
	if ((entry->mode & S_IFMT) != S_IFLNK || !entry->fullpath)
		return;
	char out[512] = { 0 };
	if (readlink(entry->fullpath, out, sizeof(out)) == -1)
		return;
	put_chars(" -> ");
	put_chars(out);
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
