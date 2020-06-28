//
// Created by James Duffy on 28/06/2020.
//

#include "iLibX.h"
//
// Created by 010010 on 15/06/2020.
//

#include "iLibX.h"
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>



void prereq(){
    system("sudo xcode-select --install");
    system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)\"");
    system("brew install pkg-config");
    system("brew install python3");
    system("brew install glib");
    system("brew install pixman");
    system("brew tap SergioBenitez/osxct");
    system("brew install aarch64-none-elf");
    system("sudo easy_install pip");
    system("pip3 install pyasn1");
    system("pip install pyasn1");
}

//iOS Exec Handler, run command return 1st line
char *ios_run_comm(char *command,char *port){
    char *com1 = "sshpass -p";
    char *com2 = "ssh -o StrictHostKeyChecking=no root@127.0.0.1 -p";
    char commout[2400];
    sprintf(commout, "%s alpine %s %s %s %s", com1, com2, port, command);
    char *com = commout;
    char out[4048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}

//return exit code of process
char *ios_run_ge(char *command, char *port){
    char *com1 = "sshpass -p";
    char *com2 = "ssh -o StrictHostKeyChecking=no root@127.0.0.1 -p";
    char *silence = ">/dev/null 2>/dev/null \; echo $?";
    char commout[2400];
    sprintf(commout, "%s alpine %s %s %s %s", com1, com2, port, command, silence);
    char *com = commout;
    char out[4048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}

//macOS Exec Handling
char *macos_run_comm(char *command){
    char com2[1024];
    strcpy(com2, command);
    char commout[1024];
    sprintf(commout, "%s", com2);
    char *com = commout;
    char out[4096];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}
int macos_run_ge(char *command){
    char *com2 = ">/dev/null 2>/dev/null;echo $?";
    char com1[2400];
    strcpy(com1, command);
    char commout[2400];
    sprintf(commout, "%s %s", com1, com2);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return atoi(out);
}

int ios_send_f(char *filetosend, char *remotedir, char *port){
    FILE *fileout;
    if((fileout = fopen(filetosend,"r"))!=NULL)
    {
        fclose(fileout);
        char commout[800];
        sprintf(commout, "sshpass -p alpine scp -P %s %s root@127.0.0.1:%s \; echo $?",port, filetosend, remotedir);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        if (atoi(out)==0){
            return 0;
        } else{
            return 1;
        }
    }

}
//ios_rec_f("/mnt1/private/etc/fstab","fstab",2222);
int ios_rec_f(char *remotefiledir, char *localfname, char *port){
    char commout[800];
    sprintf(commout, "sshpass -p alpine scp -r -P %s root@127.0.0.1:%s %s \; echo $?",port, remotefiledir, localfname);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    if (atoi(out)==0){
        return 0;
    } else{
        return 1;
    }
}