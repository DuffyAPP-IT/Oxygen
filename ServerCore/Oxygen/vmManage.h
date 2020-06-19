//
// Created by 010010 on 15/06/2020.
//

#ifndef OXYGEN_VMMANAGE_H
#define OXYGEN_VMMANAGE_H

#endif //OXYGEN_VMMANAGE_H
int heartBeat(char *port);
int generateSubVM(char *owner,char *vmid, char *hasauto, char *cbpack, char *mdir);
void startVM(char *owner, char *vmid);
int init(char *iosurl,char *devicetype,char *codename, char *rootfs, char *updatedmg);
int sendtold(char *localfile, char *cname,char *rfilename);
int clonerepo(char *pre,char *repoaddr);
int prepdirs();
