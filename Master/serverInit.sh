#!/bin/bash
echo =========================
echo oxygenCore - Main Image - DuffyAPP_IT
echo =========================

python xnu-qemu-arm64-tools/bootstrap_scripts/asn1kerneldecode.py kernelcache.release.n66 kernelcache.release.n66.asn1decoded
python3 xnu-qemu-arm64-tools/bootstrap_scripts/decompress_lzss.py kernelcache.release.n66.asn1decoded kernelcache.release.n66.out

find . -name 'DeviceTree.n66ap.im4p' -exec mv {} odtre \; >/dev/null 2>/dev/null
python xnu-qemu-arm64-tools/bootstrap_scripts/asn1dtredecode.py odtre dtree

nm kernelcache.release.n66.out > symbols.nm
export XNU_SOURCES=MASTERDIR/darwin-xnu
export KERNEL_SYMBOLS_FILE=MASTERDIR/symbols.nm
export QEMU_DIR=MASTERDIR/xnu-qemu-arm64
export NUM_BLOCK_DEVS=2
make -C xnu-qemu-arm64-tools/aleph_bdev_drv
cp ./xnu-qemu-arm64-tools/aleph_bdev_drv/bin/aleph_bdev_drv.bin ./
echo Done!

echo organising main disks...
python xnu-qemu-arm64-tools/bootstrap_scripts/asn1rdskdecode.py ./UPDATERAMDISK ./UPDATERAMDISK.out
cp ./UPDATERAMDISK.out ./hfs.main
hdiutil resize -size 6G -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil attach ./MAINDISK
echo Done!

echo enabling user ownership, syncing data, fixing permissions...
#sync ramdisk w/main
sudo diskutil enableownership /Volumes/CNAME.arm64UpdateRamDisk/
sudo rm -rf /Volumes/CNAME.arm64UpdateRamDisk/*
sudo rsync -a /Volumes/CNAME.N66OS/* /Volumes/CNAME.arm64UpdateRamDisk/
sudo chown root /Volumes/CNAME.arm64UpdateRamDisk/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64
echo Done!

echo remove data partition...
sudo rm -rf /Volumes/CNAME.arm64UpdateRamDisk/private/var/*
echo Done!


echo Cloning binpack!
git clone https://github.com/jakeajames/rootlessJB
cd rootlessJB/rootlessJB/bootstrap/tars/
tar xvf iosbinpack.tar
sudo cp -R iosbinpack64 /Volumes/CNAME.arm64UpdateRamDisk/
cd -
echo Done!

echo copy plists to disk
sudo cp bash.plist /Volumes/CNAME.arm64UpdateRamDisk/System/Library/LaunchDaemons/bash.plist
sudo cp mount_sec.plist /Volumes/CNAME.arm64UpdateRamDisk/System/Library/LaunchDaemons/mount_sec.plist
sudo cp tcptunnel.plist /Volumes/CNAME.arm64UpdateRamDisk/System/Library/LaunchDaemons/tcptunnel.plist
sudo cp dropbear.plist /Volumes/CNAME.arm64UpdateRamDisk/System/Library/LaunchDaemons/dropbear.plist
#add tunnel
sudo cp tunnel /Volumes/CNAME.arm64UpdateRamDisk/bin/tunnel
echo Done!

echo fixing signatures
touch ./tchashes
for filename in $(find /Volumes/CNAME.arm64UpdateRamDisk/iosbinpack64 -type f); do ./jtool --sig --ent $filename 2>/dev/null; done | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
sudo ./jtool --sign --ent ent.xml --inplace /Volumes/CNAME.arm64UpdateRamDisk/bin/tunnel
./jtool --sig --ent /Volumes/CNAME.arm64UpdateRamDisk/bin/tunnel | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
python3 xnu-qemu-arm64-tools/bootstrap_scripts/create_trustcache.py tchashes static_tc
echo Done!

echo patching fstab
sudo rm /Volumes/CNAME.arm64UpdateRamDisk/etc/fstab
sudo touch /Volumes/CNAME.arm64UpdateRamDisk/etc/fstab
sudo cp fstab /Volumes/CNAME.arm64UpdateRamDisk/etc/fstab
echo Done!
echo fixing launchd
sudo rm /Volumes/CNAME.arm64UpdateRamDisk/System/Library/LaunchDaemons/com.apple.mobile.keybagd.plist
sudo rm /Volumes/CNAME.arm64UpdateRamDisk/sbin/launchd
sudo cp launchd /Volumes/CNAME.arm64UpdateRamDisk/sbin/launchd
sudo ./jtool --sign --ent ent.xml --ident com.apple.xpc.launchd --inplace /Volumes/CNAME.arm64UpdateRamDisk/sbin/launchd
./jtool --sig --ent /Volumes/CNAME.arm64UpdateRamDisk/sbin/launchd | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
python3 xnu-qemu-arm64-tools/bootstrap_scripts/create_trustcache.py tchashes static_tc
echo Done!

hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk
hdiutil detach /Volumes/CNAME.N66OS

cp ./UPDATERAMDISK.out ./hfs.sec
hdiutil resize -size 6G -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil attach ./MAINDISK

sudo rm -rf /Volumes/CNAME.arm64UpdateRamDisk/*
sudo rsync -a /Volumes/CNAME.N66OS/private/var/* /Volumes/CNAME.arm64UpdateRamDisk/

sudo mkdir /Volumes/CNAME.arm64UpdateRamDisk/dropbear

hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk
hdiutil detach /Volumes/CNAME.N66OS

pwd
cd xnu-qemu-arm64
./configure --target-list=aarch64-softmmu --disable-capstone --disable-pie --disable-slirp
make -j16
cd -
echo RDY

hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk

hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk
