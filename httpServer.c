

#include<stdio.h>
#include"thread_pool.h"
#include"pub.h"
int main(int argc,char* argv[])
{
    int port = 8000;
    int st = sock_create(port);
    setdaemon();
    sock_accept(st);
    close(st);
}

