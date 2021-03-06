#pragma once

#include <types.h>
#include <block.h>

// #define O_RDONLY 0
// #define O_RDWD   1

struct file_operations;

struct file_system_type {
	const char *name;
	const struct file_operations *fops;
	struct file_system_type *next;

	int (*mount)(struct file_system_type *, unsigned long, struct block_device *);
};

int file_system_type_register(struct file_system_type *);

struct file_system_type *file_system_type_get(const char *);

////////////////////////////////
struct mount_point {
	const char *path;
	struct block_device *bdev;
	struct file_system_type *fs_type;
	struct mount_point *next;
};

#ifdef __G_BIOS__
int mount(const char *type, unsigned long flags, const char *bdev_name, const char *path);
int umount(const char *mnt);
#else
int gb_mount(const char *type, unsigned long flags, const char *bdev_name, const char *path);
int gb_umount(const char *mnt);
#endif

struct file {
	size_t pos;
	int mode;

	const struct file_operations *fops;
};

struct file_operations {
	struct file *(*open)(void *, const char *, int, int);
	int (*close)(struct file *);
	int (*read)(struct file *, void *, size_t);
	int (*write)(struct file *, const void *, size_t);
	// ...
};

#ifdef __G_BIOS__
int open(const char *const name, int flags, ...);
int close(int fd);
int read(int fd,void * buff,size_t size);
int write(int fd,const void * buff,size_t size);
#else
int gb_open(const char *const name, int flags, ...);
int gb_close(int fd);
int gb_read(int fd,void * buff,size_t size);
int gb_write(int fd,const void * buff,size_t size);
#endif
