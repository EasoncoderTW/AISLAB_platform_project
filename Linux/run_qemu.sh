mkdir -p ./shared
qemu-system-riscv64 -M virt -m 256M -nographic \
    -bios /workspace/opensbi/build/platform/generic/firmware/fw_jump.bin \
    -kernel /workspace/linux/arch/riscv/boot/Image \
    -drive file=/workspace/busybox/root.ext2,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -append "root=/dev/vda rw console=ttyS0" \
    -virtfs local,path=./shared,mount_tag=host0,security_model=mapped,id=host0