#ifndef _PTHREAD_H_
#define _PTHREAD_H_


#include"pub.h"

#include <sys/types.h>
#include <sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

extern char LOGBUF[1024];          
void get_http_command(char *http_msg,char * content);
int  make_http_content(const char *command, char **content);
void * http_thread(void *argc);

#endif