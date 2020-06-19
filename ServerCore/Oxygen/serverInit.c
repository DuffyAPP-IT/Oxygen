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

int sendtold(char *localfile, char *cname,char *rfilename){
    char sendld[6000];
    sprintf(sendld,"sudo cp %s /Volumes/%s.arm64UpdateRamDisk/System/Library/LaunchDaemons/%s",localfile,cname,rfilename);
    printf("sendld called -> %s\n",sendld);
    if(macos_run_ge(sendld)==0){
        return 0;
    } else{
        return 1;
    }

}

int init(char *iosurl,char *devicetype,char *codename, char *rootfs, char *updatedmg){
    printf("Preparing Oxygen Directory Structure\n");
    if(0==0){
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
                    if(macos_run_ge("echo Fix iURL")==0){
                        printf("Downloaded iOS...Extracting!\n");
                        if(macos_run_ge("cd oxygenData/Master && unzip rootfs.zip")==0){
                            printf("Success!\n");
//                            macos_run_comm("cd oxygenData/Master && rm rootfs.zip");
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
                                            printf("Success!\n");
                                            printf("Extracting Symbols...\n");
                                            char ESYM[2400];
                                            //fix generating symbols..
                                            sprintf(ESYM,"chmod +x oxygenData/Master/symbols.sh && cd oxygenData/Master && ./symbols.sh");
                                            if(macos_run_ge(ESYM)==0){
                                                printf("Success!\n");
                                                char decUpdate[4200];
                                                sprintf(decUpdate,"cd oxygenData/Master && python xnu-qemu-arm64-tools/bootstrap_scripts/asn1rdskdecode.py ./%s ./%s.out && cp ./%s.out ./hfs.main",updatedmg,updatedmg,updatedmg);
                                                printf("Decoding UPDATE Ramdisk...\n");
                                                if(macos_run_ge(decUpdate)==0){
                                                    printf("Success!\n");
                                                    char resmain[2400];
                                                    sprintf(resmain,"chmod +x oxygenData/Master/resizeattachmain.sh && cd oxygenData/Master && ./resizeattachmain.sh");
                                                    if(macos_run_ge(resmain)==0){
                                                        printf("Resize & Attach Success!\n");
                                                        char atroot[2400];
                                                        sprintf(atroot,"cd oxygenData/Master && hdiutil attach ./%s",rootfs);
                                                        if(macos_run_ge(atroot)==0){
                                                            printf("Mounted Root\n");
                                                            printf("Enabling Ownership For Update Disk...\n");
                                                            char eown[4000];
                                                            sprintf(eown,"sudo diskutil enableownership /Volumes/%s.arm64UpdateRamDisk/",codename);
                                                            if(macos_run_ge(eown)==0){
                                                                char rmramdisk[400];
                                                                sprintf(rmramdisk,"sudo rm -rf /Volumes/%s.arm64UpdateRamDisk/*",codename);
                                                                if(macos_run_ge(rmramdisk)==0){
                                                                    printf("Syncing Disk (This Could Take A While!\n");
                                                                    char disksync[4000];
                                                                    sprintf(disksync,"sudo rsync -a /Volumes/%s.N56N66OS/* /Volumes/%s.arm64UpdateRamDisk/",codename,codename);
                                                                    if(macos_run_ge(disksync)==0){
                                                                        printf("Success!\n");
                                                                        printf("Owning The Cache..\n");
                                                                        char own[400];
                                                                        sprintf(own, "sudo chown root /Volumes/%s.arm64UpdateRamDisk/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64",codename);
                                                                        if(macos_run_ge(own)==0){
                                                                            printf("Success!\n");
                                                                            printf("Removing Data...\n");
                                                                            char rvar[4000];
                                                                            sprintf(rvar,"sudo rm -rf /Volumes/%s.arm64UpdateRamDisk/private/var/*",codename);
                                                                            if(macos_run_ge(rvar)==0){
                                                                                printf("Data Remove Success!\nCloning rootlessjb...\n");
                                                                                if(clonerepo("cd oxygenData/Master &&","https://github.com/jakeajames/rootlessJB")==0){
                                                                                    if(macos_run_ge("cd oxygenData/Master/rootlessJB/rootlessJB/bootstrap/tars && tar xvf iosbinpack.tar")==0){
                                                                                        printf("Compression Complete!\nSending Binpack To Master IMG\n");
                                                                                        char sendbin[6000];
                                                                                        sprintf(sendbin,"cd oxygenData/Master/rootlessJB/rootlessJB/bootstrap/tars && sudo cp -R iosbinpack64 /Volumes/%s.arm64UpdateRamDisk/",codename);
                                                                                        if(macos_run_ge(sendbin)==0){
                                                                                            printf("Sent!\n");
                                                                                            if(sendtold("oxygenData/Master/bash.plist",codename,"bash.plist")==0){
                                                                                                if(sendtold("oxygenData/Master/mount_sec.plist",codename,"mount_sec.plist")==0){
                                                                                                    if(sendtold("oxygenData/Master/tcptunnel.plist",codename,"tcptunnel.plist")==0){
                                                                                                        if(sendtold("oxygenData/Master/dropbear.plist",codename,"dropbear.plist")==0){
                                                                                                            char stunnel[4000];
                                                                                                            sprintf(stunnel,"cd oxygenData/Master && sudo cp tunnel /Volumes/%s.arm64UpdateRamDisk/bin/tunnel",codename);
                                                                                                            if(macos_run_ge(stunnel)==0){
                                                                                                                printf("Copied TCP Tunnel!\n");
                                                                                                                char repval[4000];
                                                                                                                sprintf(repval,"cd oxygenData/Master && sudo sed -i '' 's/CNAME/%s/g' fixsig.sh && chmod +x fixsig.sh && ./fixsig.sh",codename);
                                                                                                                printf("Fixing Signatures & launchd\n");
                                                                                                                if(macos_run_ge(repval)==0){
                                                                                                                    printf("Fix Signatures & launchd Complete!\n");
                                                                                                                    char cpSec[6000];
                                                                                                                    sprintf(cpSec,"cd oxygenData/Master && cp ./%s.out ./hfs.sec",updatedmg);
                                                                                                                    if(macos_run_ge(cpSec)==0){
                                                                                                                        char repval2[4000];
                                                                                                                        sprintf(repval2,"cd oxygenData/Master && sudo sed -i '' 's/CNAME/%s/g' resizeattachsec.sh",codename);
                                                                                                                        macos_run_comm(repval2);
                                                                                                                        if(macos_run_ge("chmod +x oxygenData/Master/resizeattachsec.sh && cd oxygenData/Master && ./resizeattachsec.sh")==0){
                                                                                                                            printf("Complete - i think ?\n");

                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }


                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            } else{
                                                                                                printf("COPY PLIST FAIL\n");
                                                                                                return 1;
                                                                                            }

                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    } else{
                                                                        printf("FAIL Sync Disk\n");
                                                                        return 1;
                                                                    }
                                                                }
                                                            } else{
                                                                printf("Failed Enabling Disk Ownership!\n");
                                                                return 1;
                                                            }
                                                        } else{
                                                            printf("Mount Root FAIL\n");
                                                            return 1;
                                                        }

                                                    } else{
                                                        printf("Failed to resize/attach disk...\n");
                                                        return 1;
                                                    }
                                                } else{
                                                    printf("Failed to decode UPDATE Ramdisk\n");
                                                    return 1;
                                                }
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