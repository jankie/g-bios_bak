#include <unistd.h>
#include <dirent.h>
#include <block.h> //  fixme: to be removed!

static char g_cwd[PATH_MAX];

// fixme
int chdir(const char *path)
{
	DIR *dir;
	struct dirent *de;

	dir = opendir(DEV_ROOT);
	if (!dir)
		return -ENOENT;

	while ((de = readdir(dir))) {
		if (!strcmp(path, de->d_name)) {
			strncpy(g_cwd, path, PATH_MAX);
			closedir(dir);
			return 0;
		}
	}

	return -ENOENT;
}

char *getcwd(char *buff, size_t size)
{
	return strncpy(buff, g_cwd, min(size, PATH_MAX));
}

char *get_current_dir_name()
{
	return strdup(g_cwd);
}

const char *__getcwd(void)
{
	return g_cwd;
}

DIR *opendir(const char *name)
{
	DIR *dir;

	dir = zalloc(sizeof(*dir));
	if (!dir)
		return NULL;

	dir->list = bdev_get_list();
	if (!dir->list) {
		free(dir);
		return NULL;
	}

	dir->iter = dir->list->next;
	list_head_init(&dir->hash_list);

	return dir;
}

struct dirent *readdir(DIR *dir)
{
	struct dirent *de;
	struct block_device *bdev;

	assert(dir);
	if (dir->iter == dir->list)
		return NULL;

	bdev = container_of(dir->iter, struct block_device, bdev_node);

	de = zalloc(sizeof(*de));
	if (!de)
		return NULL;

	strncpy(de->d_name, bdev->name, sizeof(de->d_name));
	list_add_tail(&de->hash_node, &dir->hash_list);

	dir->iter = dir->iter->next;

	return de;
}

int closedir(DIR *dir)
{
	struct dirent *de;

	assert(dir);

	while (list_is_empty(&dir->hash_list)) {
		struct list_node *first;

		first = dir->hash_list.next;
		list_del_node(first);
		de = container_of(first, struct dirent, hash_node);
		free(de);
	}

	free(dir);
	return 0;
}