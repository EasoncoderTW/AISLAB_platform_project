#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_SIMPLE_DEVICE "simple-device"
#define SIMPLE_DEVICE(obj) \
    OBJECT_CHECK(SimpleDeviceState, (obj), TYPE_SIMPLE_DEVICE)

typedef struct SimpleDeviceState {
    SysBusDevice parent_obj; // is a system Bus device (MMIO)
    uint32_t some_property;
} SimpleDeviceState;

/* How to realize the device */
static void simple_device_realize(DeviceState *dev, Error **errp)
{
    SimpleDeviceState *s = SIMPLE_DEVICE(dev);
    // Initialize the device
    s->some_property = 0;
}

/* Enclosure the device into class */
static void simple_device_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize = simple_device_realize;
}

/* setting device info and class link */
static const TypeInfo simple_device_info = {
    .name          = TYPE_SIMPLE_DEVICE,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(SimpleDeviceState),
    .class_init    = simple_device_class_init,
};

/* register device */
static void simple_device_register_types(void)
{
    type_register_static(&simple_device_info);
}

type_init(simple_device_register_types);