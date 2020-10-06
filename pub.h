#ifndef _PUB_H_
#define _PUB_H_

//守护进程需要文件
#include<unistd.h>
#include<sys/types.h>

#include<pthread.h>

#include<stdio.h>

#include<errno.h>
#include<stdlib.h>

#include<string.h>


#include <sys/socket.h>
#include <arpa/inet.h>

#include<unistd.h>
#include"thread_pool.h"
void setdaemon();
void save_log(char * buff);
const char * get_filetype(const char *filename);
int sock_create(int port);
int sock_accept(int st);
int get_file_content(const char * filename,char * content[]);

#endif