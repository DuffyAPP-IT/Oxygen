#!/bin/bash
echo =========================
echo oxygenCore - Main Image - DuffyAPP_IT
echo =========================


echo Creating Directory Structure...
#Git clone relevant repos
mkdir oxygenData oxygenData/Master oxygenData/userData >/dev/null 2>/dev/null
cd oxygenData/Master
echo Done!
# #INSSTALL PREREQS IN C....
# # sudo xcode-select --install
# # INSTALL BREW

echo Cloning XNU-QEMU,DARWIN-XNU...
git clone https://github.com/alephsecurity/xnu-qemu-arm64.git
git clone https://github.com/alephsecurity/xnu-qemu-arm64-tools
git clone https://github.com/apple/darwin-xnu.git
brew install pkg-config
echo Done!
brew install glib
echo Done!
brew install pixman
echo Done!

echo Installing Other Prerequesites...
brew tap SergioBenitez/osxct
echo Done!
brew install aarch64-none-elf
echo Done!
sudo easy_install pip
echo Done!
pip3 install pyasn1
echo Done!
pip install pyasn1
echo Done!

echo Downloading iOS...
##Retrieve iOS Image
curl http://updates-http.cdn-apple.com/2018FallFCS/fullrestores/091-91479/964118EC-D4BE-11E8-BC75-A45C715A3354/iPhone_5.5_12.1_16B92_Restore.ipsw --output rootfs.zip
echo Done!


echo Unzipping iOS...
unzip rootfs.zip
echo Done!

echo "Find, decode & decompress kernelcache..."
python xnu-qemu-arm64-tools/bootstrap_scripts/asn1kerneldecode.py kernelcache.release.n66 kernelcache.release.n66.asn1decoded
python3 xnu-qemu-arm64-tools/bootstrap_scripts/decompress_lzss.py kernelcache.release.n66.asn1decoded kernelcache.release.n66.out
echo Done!


echo Find, decode devicetree...
find . -name 'DeviceTree.n66ap.im4p' -exec mv {} odtre \; >/dev/null 2>/dev/null
python xnu-qemu-arm64-tools/bootstrap_scripts/asn1dtredecode.py odtre dtree
echo Done!


echo "extracting symbols & compile bdev_drv..."
nm kernelcache.release.n66.out > symbols.nm
export XNU_SOURCES=~/Documents/Oxygen/oxygenData/Master/darwin-xnu
export KERNEL_SYMBOLS_FILE=~/Documents/Oxygen/oxygenData/Master/symbols.nm
export QEMU_DIR=~/Documents/Oxygen/oxygenData/Master/xnu-qemu-arm64
export NUM_BLOCK_DEVS=2
make -C xnu-qemu-arm64-tools/aleph_bdev_drv
cp ./xnu-qemu-arm64-tools/aleph_bdev_drv/bin/aleph_bdev_drv.bin ./
echo Done!

echo organising main disks...
python xnu-qemu-arm64-tools/bootstrap_scripts/asn1rdskdecode.py ./048-32651-104.dmg ./048-32651-104.dmg.out
cp ./048-32651-104.dmg.out ./hfs.main
hdiutil resize -size 6G -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil attach ./048-31952-103.dmg
echo Done!

echo enabling user ownership, syncing data, fixing permissions...
#sync ramdisk w/main
sudo diskutil enableownership /Volumes/PeaceB16B92.arm64UpdateRamDisk/
sudo rm -rf /Volumes/PeaceB16B92.arm64UpdateRamDisk/*
sudo rsync -a /Volumes/PeaceB16B92.N56N66OS/* /Volumes/PeaceB16B92.arm64UpdateRamDisk/
sudo chown root /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64
echo Done!

echo remove data partition...
sudo rm -rf /Volumes/PeaceB16B92.arm64UpdateRamDisk/private/var/*
echo Done!


echo Cloning binpack!
git clone https://github.com/jakeajames/rootlessJB
cd rootlessJB/rootlessJB/bootstrap/tars/
tar xvf iosbinpack.tar
sudo cp -R iosbinpack64 /Volumes/PeaceB16B92.arm64UpdateRamDisk/
cd -
echo Done!

echo copy plists to disk
sudo cp bash.plist /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/bash.plist
sudo cp mount_sec.plist /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/mount_sec.plist
sudo cp tcptunnel.plist /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/tcptunnel.plist
sudo cp dropbear.plist /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/dropbear.plist
#add tunnel
sudo cp tunnel /Volumes/PeaceB16B92.arm64UpdateRamDisk/bin/tunnel
echo Done!

echo fixing signatures
touch ./tchashes
for filename in $(find /Volumes/PeaceB16B92.arm64UpdateRamDisk/iosbinpack64 -type f); do ./jtool --sig --ent $filename 2>/dev/null; done | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
sudo ./jtool --sign --ent ent.xml --inplace /Volumes/PeaceB16B92.arm64UpdateRamDisk/bin/tunnel
./jtool --sig --ent /Volumes/PeaceB16B92.arm64UpdateRamDisk/bin/tunnel | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
python3 xnu-qemu-arm64-tools/bootstrap_scripts/create_trustcache.py tchashes static_tc
echo Done!

echo patching fstab
sudo rm /Volumes/PeaceB16B92.arm64UpdateRamDisk/etc/fstab
sudo touch /Volumes/PeaceB16B92.arm64UpdateRamDisk/etc/fstab
sudo cp fstab /Volumes/PeaceB16B92.arm64UpdateRamDisk/etc/fstab
echo Done!
echo fixing launchd
sudo rm /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/com.apple.mobile.keybagd.plist
sudo rm /Volumes/PeaceB16B92.arm64UpdateRamDisk/sbin/launchd
sudo cp launchd /Volumes/PeaceB16B92.arm64UpdateRamDisk/sbin/launchd
sudo ./jtool --sign --ent ent.xml --ident com.apple.xpc.launchd --inplace /Volumes/PeaceB16B92.arm64UpdateRamDisk/sbin/launchd
./jtool --sig --ent /Volumes/PeaceB16B92.arm64UpdateRamDisk/sbin/launchd | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
python3 xnu-qemu-arm64-tools/bootstrap_scripts/create_trustcache.py tchashes static_tc
echo Done!

hdiutil detach /Volumes/PeaceB16B92.arm64UpdateRamDisk
hdiutil detach /Volumes/PeaceB16B92.N56N66OS

cp ./048-32651-104.dmg.out ./hfs.sec
hdiutil resize -size 6G -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil attach ./048-31952-103.dmg

sudo rm -rf /Volumes/PeaceB16B92.arm64UpdateRamDisk/*
sudo rsync -a /Volumes/PeaceB16B92.N56N66OS/private/var/* /Volumes/PeaceB16B92.arm64UpdateRamDisk/

sudo mkdir /Volumes/PeaceB16B92.arm64UpdateRamDisk/dropbear

hdiutil detach /Volumes/PeaceB16B92.arm64UpdateRamDisk
hdiutil detach /Volumes/PeaceB16B92.N56N66OS

pwd
cd xnu-qemu-arm64
./configure --target-list=aarch64-softmmu --disable-capstone --disable-pie --disable-slirp
make -j16
cd -
echo RDY
xnu-qemu-arm64/aarch64-softmmu/qemu-system-aarch64 -M iPhone6splus-n66-s8000,kernel-filename=kernelcache.release.n66.out,dtb-filename=dtree,driver-filename=aleph_bdev_drv.bin,qc-file-0-filename=hfs.main,qc-file-1-filename=hfs.sec,tc-filename=static_tc,kern-cmd-args="debug=0x8 kextlog=0xfff cpus=1 rd=disk0 serial=2",xnu-ramfb=on -cpu max -m 6G -serial mon:stdio