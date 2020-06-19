nm kernelcache.release.N66.out > symbols.nm
export XNU_SOURCES=~/Documents/Oxygen/oxygenData/Master/darwin-xnu
export KERNEL_SYMBOLS_FILE=~/Documents/Oxygen/oxygenData/Master/symbols.nm
export QEMU_DIR=~/Documents/Oxygen/oxygenData/Master/xnu-qemu-arm64
export NUM_BLOCK_DEVS=2
make -C xnu-qemu-arm64-tools/aleph_bdev_drv
cp ./xnu-qemu-arm64-tools/aleph_bdev_drv/bin/aleph_bdev_drv.bin ./