//
// Created by 010010 on 15/06/2020.
//

#include "vmManage.h"
#include <stdio.h>
#include <sys/wait.h>
#include "iLibX.h"
#include <stdlib.h>
#define BUFF 800

//Pass port of VM, returns 0 if VM active and accepting SSH transmissions
int heartBeat(char *port){
    char *ping = "nc -z";
    char *silence = ">/dev/null 2>/dev/null";
    char command[800];
    sprintf(command,"%s 127.0.0.1 %s %s",ping,port,silence);
    FILE * shell;
    shell = popen(command, "r");
    int r = pclose(shell);
    if(WEXITSTATUS(r)==0)
    {
        return 0;
    } else {
        return 1;
    }
}

//add database value checks (just for double checking...)
//VMID should be a 4 digit number

int generateSubVM(char *owner,char *vmid, char *hasauto, char *cbpack, char *mdir){
    printf("Debugging ON\n");
    //Create User Directory
    char ownercreate[BUFF];
    sprintf(ownercreate,"mkdir Users/%s Users/%s/%s",owner,owner,vmid);
    if(macos_run_ge(ownercreate)==0){
        printf("Directory Structure Create Success\n");
        //Copy Master Files
        char copyMaster[BUFF];
        sprintf(copyMaster,"cp -R Master/* Users/%s/%s/",owner,vmid);
        if(macos_run_ge(copyMaster)==0){
            printf("Master Image Copy Success\n");
            //Set Custom Port
            char attach[BUFF];
            sprintf(attach,"cd Users/%s/%s/ && hdiutil attach -imagekey diskimage-class=CRawDiskImage hfs.main",owner,vmid);
            system(attach);
            char repval[4000];
            sprintf(repval,"sudo sed -i '' 's/2244/%s/g' /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/tcptunnel.plist",vmid);
            printf("REPVAL IS -> %s",repval);
            system("cp /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/tcptunnel.plist ~/t.plist");
            system(repval);
            system("cp /Volumes/PeaceB16B92.arm64UpdateRamDisk/System/Library/LaunchDaemons/tcptunnel.plist ~/patched.plist");
            system("hdiutil detach /Volumes/PeaceB16B92.arm64UpdateRamDisk");
            printf("Eject Succes\n");
            system(attach);
            system("hdiutil detach /Volumes/PeaceB16B92.arm64UpdateRamDisk");
            char attachsec[BUFF];
            sprintf(attachsec,"cd Users/%s/%s/ && hdiutil attach -imagekey diskimage-class=CRawDiskImage hfs.main",owner,vmid);
            system(attachsec);
            system("hdiutil detach /Volumes/PeaceB16B92.arm64UpdateRamDisk");
        } else{
            printf("Error Copying Master Image..\n");
        }
    }
}

void startVM(char *owner, char *vmid){
    printf("STARTVM CALLED");
    char vmStart[4000];
    sprintf(vmStart,"cd Users/%s/%s/ && xnu-qemu-arm64/aarch64-softmmu/qemu-system-aarch64 -M iPhone6splus-n66-s8000,kernel-filename=kernelcache.release.n66.out,dtb-filename=dtree,driver-filename=aleph_bdev_drv.bin,qc-file-0-filename=hfs.main,qc-file-1-filename=hfs.sec,tc-filename=static_tc,kern-cmd-args=\"debug=0x8 kextlog=0xfff cpus=1 rd=disk0 serial=2\",xnu-ramfb=on -cpu max -m 6G -serial mon:stdio",owner,vmid);
    printf("%s",vmStart);
    system(vmStart);
}