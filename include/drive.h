#pragma once

#include <list.h>
#include <block.h>

struct generic_drive
{
	struct block_device blk_dev;

	int (*get_block)(struct generic_drive *drive, int start, u8 buff[]);
	int (*put_block)(struct generic_drive *drive, int start, const u8 buff[]);

	size_t sect_size; // fixme: move to block_device

	union
	{
		struct list_node master_node;
		struct list_node slave_node;
	};

	union
	{
		struct list_node slave_list;
		struct generic_drive *master;
	};
};

int generic_drive_register(struct generic_drive *drive);
