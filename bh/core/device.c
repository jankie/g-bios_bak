#include <device.h>
#include <list.h>

int device_register(struct device *dev)
{
	int ret;
	struct list_node *iter;
	struct bus *bus = dev->bus;

	// step 1:
	list_add_tail(&dev->bus_node, &bus->dev_list);

	// step 2:
	list_for_each(iter, &bus->drv_list) {
		struct driver *drv;

		drv = container_of(iter, struct driver, bus_node);
		if (bus->match(dev, drv)) {
			list_add_tail(&dev->drv_node, &drv->dev_list);

			// step 3:
			dev->drv = drv;
			ret = drv->init(dev);
			if (!ret)
				return 0;

			dev->drv = NULL;
		}
	}

	return 0;
}

int driver_register(struct driver *drv)
{
	int ret;
	struct list_node *iter;
	struct bus *bus = drv->bus;

	// printf("%s() line %d\n", __func__, __LINE__);
	list_add_tail(&drv->bus_node, &bus->drv_list);

	list_for_each(iter, &bus->dev_list) {
		struct device *dev;

		// printf("%s() line %d\n", __func__, __LINE__);
		dev = container_of(iter, struct device, bus_node);
		if (bus->match(dev, drv)) {
			list_add_tail(&dev->drv_node, &drv->dev_list);

			dev->drv = drv;
			ret = drv->init(dev);
			if (ret < 0)
				dev->drv = NULL;
		}
	}

	return 0;
}
