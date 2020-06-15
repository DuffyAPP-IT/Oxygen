//
// Created by 010010 on 15/06/2020.
//

#include "vmManage.h"
#include <stdio.h>
#include <sys/wait.h>

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

int generateSubVM(char *owner,char *vmid, char *hasauto, char *cbpack, char *mdir){
    
}