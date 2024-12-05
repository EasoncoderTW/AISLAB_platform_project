/*
 * Virtual Adder Device Driver
 *
 * Copyright 2024 Eason Yeh <easonyehmobile1@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/init.h>

/* Register map */
#define REG_ID                 0x0
#define CHIP_ID                0xf001

#define REG_INIT               0x4
#define CHIP_EN                BIT(1)

#define REG_A                  0x8
#define REG_B                  0xc
#define REG_O                  0x10

#define REG_INT_STATUS         0x14
#define INT_ENABLED            BIT(0)
#define INT_BUFFER_DEQ         BIT(1)


struct virt_adder {
    struct device *dev;
    void __iomem *base;
};

static ssize_t adder_show_id(struct device *dev,
              struct device_attribute *attr, char *buf)
{
    struct virt_adder *adder = dev_get_drvdata(dev);
    u32 val = readl_relaxed(adder->base + REG_ID);

    return scnprintf(buf, PAGE_SIZE, "Chip ID: 0x%.x\n", val);
}

static ssize_t adder_show_dataA(struct device *dev,
               struct device_attribute *attr, char *buf)
{
    struct virt_adder *adder = dev_get_drvdata(dev);
    u32 val = readl_relaxed(adder->base + REG_A);

    return scnprintf(buf, PAGE_SIZE, "Data A buffer: 0x%.x\n", val);
}

static ssize_t adder_show_dataB(struct device *dev,
               struct device_attribute *attr, char *buf)
{
    struct virt_adder *adder = dev_get_drvdata(dev);
    u32 val = readl_relaxed(adder->base + REG_B);

    return scnprintf(buf, PAGE_SIZE, "Data B buffer: 0x%.x\n", val);
}

static ssize_t adder_show_dataO(struct device *dev,
               struct device_attribute *attr, char *buf)
{
    struct virt_adder *adder = dev_get_drvdata(dev);
    u32 val = readl_relaxed(adder->base + REG_O);

    return scnprintf(buf, PAGE_SIZE, "Data O buffer: 0x%.x\n", val);
}

static ssize_t adder_store_dataA(struct device *dev,
                struct device_attribute *attr,
                const char *buf, size_t len)
{
    struct virt_adder *adder = dev_get_drvdata(dev);
    unsigned long val;

    if (kstrtoul(buf, 0, &val))
        return -EINVAL;

    writel_relaxed(val, adder->base + REG_A);

    return len;
}

static ssize_t adder_store_dataB(struct device *dev,
                struct device_attribute *attr,
                const char *buf, size_t len)
{
    struct virt_adder *adder = dev_get_drvdata(dev);
    unsigned long val;

    if (kstrtoul(buf, 0, &val))
        return -EINVAL;

    writel_relaxed(val, adder->base + REG_B);

    return len;
}


static DEVICE_ATTR(id, S_IRUGO, adder_show_id, NULL);
static DEVICE_ATTR(dataO, S_IRUGO, adder_show_dataO, NULL);
static DEVICE_ATTR(dataA, S_IRUGO | S_IWUSR, adder_show_dataA, adder_store_dataA);
static DEVICE_ATTR(dataB, S_IRUGO | S_IWUSR, adder_show_dataB, adder_store_dataB);

static struct attribute *adder_attributes[] = {
    &dev_attr_id.attr,
    &dev_attr_dataO.attr,
    &dev_attr_dataA.attr,
    &dev_attr_dataB.attr,
    NULL,
};

static const struct attribute_group adder_attr_group = {
    .attrs = adder_attributes,
};

static void adder_init(struct virt_adder *adder)
{
    printk(KERN_INFO "Adder module init.\n");
    writel_relaxed(CHIP_EN, adder->base + REG_INIT);
}

static irqreturn_t adder_irq_handler(int irq, void *data)
{
    printk(KERN_INFO "Adder module irq_handler.\n");
    struct virt_adder *adder = (struct virt_adder *)data;
    u32 status;

    status = readl_relaxed(adder->base + REG_INT_STATUS);

    if (status & INT_ENABLED)
        dev_info(adder->dev, "HW is enabled\\n");

    if (status & INT_BUFFER_DEQ)
        dev_info(adder->dev, "Command buffer is dequeued\\n");

    return IRQ_HANDLED;
}

static int adder_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "Adder module probe.\n");
    struct device *dev = &pdev->dev;
    struct resource *res;
    struct virt_adder *adder;
    int ret;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
        return -ENOMEM;

    adder = devm_kzalloc(dev, sizeof(*adder), GFP_KERNEL);
    if (!adder)
        return -ENOMEM;

    adder->dev = dev;
    adder->base = devm_ioremap(dev, res->start, resource_size(res));
    if (!adder->base)
        return -EINVAL;

    res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
    if (res) {
        ret = devm_request_irq(dev, res->start, adder_irq_handler,
                       IRQF_TRIGGER_HIGH, "adder_irq", adder);
        if (ret)
            return ret;
    }

    platform_set_drvdata(pdev, adder);

    adder_init(adder);

    return sysfs_create_group(&dev->kobj, &adder_attr_group);
}

static int adder_remove(struct platform_device *pdev)
{
    struct virt_adder *adder = platform_get_drvdata(pdev);

    sysfs_remove_group(&adder->dev->kobj, &adder_attr_group);
    return 0;
}

static const struct of_device_id adder_of_match[] = {
    { .compatible = "adder", },
    { }
};
MODULE_DEVICE_TABLE(of, adder_of_match);

static struct platform_driver adder_driver = {
    .probe = adder_probe,
    .remove = adder_remove,
    .driver = {
        .name = "adder",
        .of_match_table = adder_of_match,
    },
};
module_platform_driver(adder_driver);

MODULE_DESCRIPTION("Virtual Adder Driver");
MODULE_AUTHOR("Eason Yeh");
MODULE_LICENSE("Dual BSD/GPL");

/*static int adder_init(void) {
	    printk(KERN_INFO "Adder module init.\n");
	        return 0;
}

static void adder_exit(void)
{
	    printk(KERN_INFO "Adder module exit.\n");
}

module_init(adder_init);
module_exit(adder_exits);*/

