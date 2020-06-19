touch ./tchashes
for filename in $(find /Volumes/CNAME.arm64UpdateRamDisk/iosbinpack64 -type f); do ./jtool --sig --ent $filename 2>/dev/null; done | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
sudo ./jtool --sign --ent ent.xml --inplace /Volumes/CNAME.arm64UpdateRamDisk/bin/tunnel
./jtool --sig --ent /Volumes/CNAME.arm64UpdateRamDisk/bin/tunnel | grep CDHash | cut -d' ' -f6 | cut -c 1-40 >> ./tchashes
python3 xnu-qemu-arm64-tools/bootstrap_scripts/create_trustcache.py tchashes static_tc

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