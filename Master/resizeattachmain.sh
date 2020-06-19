hdiutil resize -size 6G -imagekey diskimage-class=CRawDiskImage ./hfs.main
hdiutil attach -imagekey diskimage-class=CRawDiskImage ./hfs.main