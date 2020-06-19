hdiutil resize -size 6G -imagekey diskimage-class=CRawDiskImage ./hfs.sec
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.sec

sudo rm -rf /Volumes/CNAME.arm64UpdateRamDisk/*
sudo rsync -a /Volumes/CNAME.N56N66OS/private/var/* /Volumes/CNAME.arm64UpdateRamDisk/
sudo mkdir /Volumes/CNAME.arm64UpdateRamDisk/dropbear

hdiutil detach /Volumes/CNAME.arm64UpdateRamDisk
hdiutil detach /Volumes/CNAME.N56N66OS