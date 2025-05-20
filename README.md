# Testing the file system
```bash
dd if=/dev/zero of=fat32.img bs=1M count=100
mkfs.vfat -F 32 fat32.img
sudo mount -o loop fat32.img /mnt
lsblk
ls /mnt
sudo umount /mnt
hexdump -C fat32.img
```