//
// Created by 010010 on 15/06/2020.
//

#ifndef OXYGEN_ILIBX_H
#define OXYGEN_ILIBX_H

#endif //OXYGEN_ILIBX_H

char *ios_run_comm(char *command,char *port);
char *ios_run_ge(char *command, char *port);
char *macos_run_comm(char *command);
int macos_run_ge(char *command);
int ios_send_f(char *filetosend, char *remotedir, char *port);
int ios_rec_f(char *remotefiledir, char *localfname, char *port);