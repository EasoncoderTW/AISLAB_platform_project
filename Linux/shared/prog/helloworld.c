#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

int main() {
    int fd;
    uint32_t value = 0x12345678;

    fd = open("/dev/mmio_platform_device", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    
    for(int i = 0;i<10;i++)
    {
        if (read(fd, &value, sizeof(value)) < 0) {
            perror("Failed to read from the device");
            close(fd);
            return errno;
        }

        printf("Read 0x%x from the device\n", value);
    }
    close(fd);
    return 0;
}
