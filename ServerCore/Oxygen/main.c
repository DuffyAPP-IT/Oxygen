#define VERSION 0
#define MVERSION 2
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "vmManage.h"


int main(int argc, char *argv[]) {
    printf("//////////////////////\n");
    printf("Project Oxygen V%d.%d\n",VERSION,MVERSION);
    printf("//////////////////////\n");
    if(argv[1]==0){
        printf("Invalid Request To Oxygen...\n");
        return 1;
    }else if(strcmp(argv[1],"-h")==0) {
        printf("Check Oxygen Instructions -> Oxygen.pdf\n");
        return 0;
    }else if(strcmp(argv[1],"-i")==0) {
        printf("Initialising Oxygen Server...\n");
<<<<<<< Updated upstream
        system("mkdir oxygenData oxygenData/Users");
        system("cp Oxygen oxygenData/Oxygen");
        system("cp Master.zip oxygenData/Master.zip");
        system("cd oxygenData && unzip Master.zip");
        system("./serverInit.sh");
=======
        init("https://updates.cdn-apple.com/2019FallFCS/fullrestores/041-81340/FC3F00A8-F4E5-11E9-A8C8-B6C134390AC5/iPhone_5.5_13.2_17B84_Restore.ipsw","N66","YukonB17B84","048-90358-101.dmg","048-90336-105.dmg");
>>>>>>> Stashed changes
        return 0;
    }else if(strcmp(argv[1],"-c")==0){
        printf("Oxygen -> Create...\n");
        if(argv[2]==0){
            printf("Owner ID Not Supplied\n");
            return 1;
        } else if(argv[3]==0){
            printf("VM ID Not Supplied...\n");
            return 1;
        } else if(argv[4]==0){
            printf("HasAutoRun Not Supplied...\n");
            return 1;
        } else if(argv[5]==0){
            printf("CBPack Info Not Supplied...\n");
            return 1;
        }
        printf("Creating!\n");
        generateSubVM(argv[2],argv[3],argv[4],argv[2],"Master/*");
    }
    else if(strcmp(argv[1],"-s")==0){
        //verify owner and vmid are present...
        printf("Starting via OWNERID -> %s -> VMID -> %s\n",argv[2], argv[3]);
        startVM(argv[2],argv[3]);
        return 0;
    }

        //generate random number to return as main output? file with info maybe
    return 0;
}
