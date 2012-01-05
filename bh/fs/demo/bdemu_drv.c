#include <init.h>
#include <stdio.h>
#include <block.h>

static struct block_device g_bdev[] = {
	{
		.name = "/dev/sdz1",
		.map  = "/maxwit/image/hd_fat32.img",
	}, {
		.name = "/dev/sdz2",
#ifdef CONFIG_MAP_FILE
		.map  = CONFIG_MAP_FILE,
#else
		.map  = "/maxwit/image/hd_ext2.img",
#endif
	},
};

static __INIT__ int bdemu_init()
{
	int i, ret;

	for (i = 0; i < sizeof(g_bdev) / sizeof(g_bdev[0]); i++) {
		ret = block_device_register(&g_bdev[i]);
		if (!ret)
			printf("block device \"%s\" registered! map file = \"%s\"\n",
				g_bdev[i].name, g_bdev[i].map);
	}

	return 0;
}

module_init(bdemu_init);