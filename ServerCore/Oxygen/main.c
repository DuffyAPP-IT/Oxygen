#define VERSION 0
#define MVERSION 4
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "vmManage.h"
#include "iLibX.h"


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
    }else if(strcmp(argv[1],"-p")==0) {
        prereq();
        return 0;
    }
    else if(strcmp(argv[1],"-i")==0) {
        printf("Initialising Oxygen Server...\n");
        init("http://updates-http.cdn-apple.com/2018FallFCS/fullrestores/091-91479/964118EC-D4BE-11E8-BC75-A45C715A3354/iPhone_5.5_12.1_16B92_Restore.ipsw","N66","Peace16A366","048-08888-230.dmg","048-08422-242.dmg");
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
