/*
 * Virtual MMIO Bus Device Driver
 *
 * Copyright 2024 Eason Yeh <easonyehmobile1@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/of.h>

/* MMIO */
#define DRIVER_NAME "mmio_bus"
#define MMIO_SIZE 0x6000000

static int major;
static struct cdev mmio_cdev;
static void __iomem *mmio_base;

static ssize_t mmio_read(struct file *filp, char __user *buf, size_t len, loff_t *offset)
{
    uint32_t value;

    if (*offset >= MMIO_SIZE) {
        return 0;
    }

    if (*offset + len > MMIO_SIZE) {
        len = MMIO_SIZE - *offset;
    }

    value = ioread32(mmio_base + *offset);

    if (copy_to_user(buf, &value, len)) {
        return -EFAULT;
    }

    *offset += len;
    return len;
}

static ssize_t mmio_write(struct file *filp, const char __user *buf, size_t len, loff_t *offset)
{
    uint32_t value;

    if (*offset >= MMIO_SIZE) {
        return 0;
    }

    if (*offset + len > MMIO_SIZE) {
        len = MMIO_SIZE - *offset;
    }

    if (copy_from_user(&value, buf, len)) {
        return -EFAULT;
    }

    iowrite32(value, mmio_base + *offset);

    *offset += len;
    return len;
}

static const struct file_operations mmio_fops = {
    .owner = THIS_MODULE,
    .read = mmio_read,
    .write = mmio_write,
};

static int mmio_bus_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "MMIO Bus module probe.\n");
    struct resource *res;
    dev_t dev;
    int ret;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "Failed to get memory resource\n");
        return -ENODEV;
    }

    mmio_base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(mmio_base)) {
        dev_err(&pdev->dev, "Failed to remap memory resource\n");
        return PTR_ERR(mmio_base);
    }

    ret = alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME);
    if (ret < 0) {
        dev_err(&pdev->dev, "Failed to allocate char device region\n");
        return ret;
    }

    major = MAJOR(dev);
    cdev_init(&mmio_cdev, &mmio_fops);

    ret = cdev_add(&mmio_cdev, dev, 1);
    if (ret < 0) {
        dev_err(&pdev->dev, "Failed to add char device\n");
        unregister_chrdev_region(dev, 1);
        return ret;
    }

    dev_info(&pdev->dev, "MMIO device initialized with major number %d\n", major);
    return 0;
}

static int mmio_bus_remove(struct platform_device *pdev)
{
    dev_t dev = MKDEV(major, 0);

    cdev_del(&mmio_cdev);
    unregister_chrdev_region(dev, 1);
    dev_info(&pdev->dev, "MMIO Bus exited\n");
    return 0;
}

static const struct of_device_id mmio_bus_of_match[] = {
    { .compatible = "mmio_bus", },
    { }
};
MODULE_DEVICE_TABLE(of, mmio_bus_of_match);

static struct platform_driver mmio_bus_driver = {
    .probe = mmio_bus_probe,
    .remove = mmio_bus_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = mmio_bus_of_match,
    },
};
module_platform_driver(mmio_bus_driver);

MODULE_DESCRIPTION("Virtual MMIO Bus Driver");
MODULE_AUTHOR("Eason Yeh");
MODULE_LICENSE("Dual BSD/GPL");


