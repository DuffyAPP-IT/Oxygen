//
// Created by 010010 on 18/06/2020.
//

#include <stdio.h>
#include "iLibX.h"
#include "vmManage.h"

int prepdirs(){
    macos_run_comm("rm -rf oxygenData");
    if(macos_run_ge("mkdir oxygenData oxygenData/Users")==0){
        macos_run_comm("cp Master.zip oxygenData/Master.zip");
        macos_run_ge("cd oxygenData && unzip Master.zip");
        return 0;
    } else{
        return 1;
    }
}

int clonerepo(char *pre,char *repoaddr){
    char clone[1200];
    sprintf(clone,"%s git clone %s",pre,repoaddr);
    if(macos_run_ge(clone)==0){
        return 0;
    } else{
        return 1;
    }
}

int init(char *iosurl,char *devicetype,char *codename){
    printf("Preparing Oxygen Directory Structure\n");
    if(prepdirs()==0){
        printf("Success!\n");
        printf("Cloning XNU-QEMU-64\n");
        if(clonerepo("cd oxygenData/Master &&","https://github.com/alephsecurity/xnu-qemu-arm64.git")==0){
            printf("Success!\n");
            printf("Cloning XNU-QEMU-64-Tools\n");
            if(clonerepo("cd oxygenData/Master &&","https://github.com/alephsecurity/xnu-qemu-arm64-tools")==0){
                printf("Success!\n");
                printf("Cloning Darwin XNU\n");
                if(clonerepo("cd oxygenData/Master &&","https://github.com/apple/darwin-xnu.git")==0){
                    printf("Success!\n");
                    printf("Downloading iOS for %s\n",devicetype);
                    char iURL[2400];
                    sprintf(iURL,"cd oxygenData/Master && curl %s --output rootfs.zip",iosurl);
                    if(macos_run_ge(iURL)==0){
                        printf("Downloaded iOS...Extracting!\n");
                        if(macos_run_ge("cd oxygenData/Master && unzip rootfs.zip")==0){
                            printf("Success!\n");
                            macos_run_comm("cd oxygenData/Master && rm rootfs.zip");
                            printf("Decoding Kernelcache Images...\n");
                            char decKernel[2400];
                            sprintf(decKernel,"cd oxygenData/Master && python xnu-qemu-arm64-tools/bootstrap_scripts/asn1kerneldecode.py kernelcache.release.N66 kernelcache.release.N66.asn1decoded");
                            if(macos_run_ge(decKernel)==0){
                                printf("Decoded Kernel - Decompressing...\n");
                                char deccKernel[4800];
                                sprintf(deccKernel,"cd oxygenData/Master && python3 xnu-qemu-arm64-tools/bootstrap_scripts/decompress_lzss.py kernelcache.release.N66.asn1decoded kernelcache.release.N66.out");
                                if(macos_run_ge(deccKernel)==0){
                                    printf("Success!\n");
                                    printf("Moving DeviceTree...\n");
                                    char DTF[2400];
                                    sprintf(DTF,"cd oxygenData/Master && find . -name 'DeviceTree.n66ap.im4p' -exec mv {} odtre \\;");
                                    if(macos_run_ge(DTF)==0){
                                        printf("Success!\n");
                                        printf("Decoding Device Tree...\n");
                                        if(macos_run_ge("cd oxygenData/Master && python xnu-qemu-arm64-tools/bootstrap_scripts/asn1dtredecode.py odtre dtree")==0){
                                            printf("Success!");
                                            printf("Extracting Symbols...\n");
                                            char ESYM[2400];
                                            //fix generating symbols..
                                            sprintf(ESYM,"chmod +x oxygenData/Master/symbols.sh && cd oxygenData/Master && ./symbols.sh");
                                            if(macos_run_ge(ESYM)==0){
                                                printf("Success!\n");


                                            } else{
                                                printf("Failed Extracting Symbols\n");
                                                return 1;
                                            }

                                        } else{
                                            printf("Failed Device Tree Decoding...\n");
                                            return 1;
                                        }

                                    } else{
                                        printf("Couldnt find DTREE for specific Device Type...\n");
                                        return 1;
                                    }
                                } else{
                                    printf("FAIL\n");
                                    return 1;
                                }
                            } else{
                                printf("Decoding Kernel Failed... Check Device Type?\n");
                            }
                        } else{
                            printf("Failed Extraction...\n");
                            return 1;
                        }


                    } else{
                        printf("Downloading this iOS image failed... check the URL?\n");
                        return 1;
                    }


                } else{
                    printf("ERR Cloning Darwin XNU\n");
                    return 1;
                }
            } else{
                printf("ERR Cloning XNU-QEMU-64-Tools");
                return 1;
            }
        } else{
            printf("ERR Cloning XNU-QEMU-64");
        }
    } else{
        printf("ERR Creating Directories...\n");
        return 1;
    }


}