//http 消息头
#define HEAD "HTTP/1.0 200 OK\n\
Content-Type: %s\n\
Transfer-Encoding: chunked\n\
Connection: Keep-Alive\n\
Accept-Ranges:bytes\n\
Content-Length:%d\n\n"
//http 消息尾
#define TAIL "\n\n"

#include "pub.h"
#include "thread_pool.h"
char buff[1024];
void get_http_command(char * http_msg,char * command)
{
    char * p_end = http_msg;
    char * p_start = http_msg;
    while(* p_start)
    {
        if(* p_start == '/')
        {
            break;
        }
        p_start++;
    }
    p_start++;
    p_end = strchr(http_msg, '\n');
    while(p_end != p_start)
    {
        if(* p_end == ' ')
        {
            break;
        }
        p_end--;
    }
    strncpy(command, p_start, p_end - p_start);
    sprintf(buff,"TTTT:%s\n\n\n",command);
    save_log(buff);
}

int make_http_content(const char *command, char **content)
{
    char *file_buf;
	int file_length;
	char headbuf[1024];

    if(command[0]==0)
    {
        file_length = get_file_content("index.html",&file_buf);
    }
    else
    {
        file_length = get_file_content(command, &file_buf);
    }
    if (file_length == 0)
	{
		return 0;
	}

    memset(headbuf,0,sizeof(headbuf));
    sprintf(headbuf,HEAD,get_filetype(command),file_length);

    int iheadlen = strlen(headbuf);
    int itaillen = strlen(TAIL);
    int isumlen = iheadlen + itaillen + file_length;
    *content = (char *) malloc(isumlen);
    if(*content == NULL)
    {
        memset(buff,0,sizeof(buff));
		sprintf(buff,"malloc failed %s\n", strerror(errno));
		save_log(buff);
    }
    char * tmp = *content;
    memcpy(tmp,headbuf,iheadlen);
    memcpy(tmp + iheadlen,file_buf,file_length);
    memcpy(tmp + file_length + iheadlen,TAIL,itaillen);
    if (file_buf)
	{
		free(file_buf);
	}
    return isumlen;
}

void * http_thread(void *argc)
{
    if(argc == NULL)
        return NULL;
        int st = *(int *)argc;
    free((int *)argc);
    char buf[1024];
    memset(buf,0,sizeof(buf));
    int recvlen = recv(st,buf,sizeof(buf),0);
    if(recvlen > 0)
    {
        char command[1024];
        memset(command,0,sizeof(command));
        get_http_command(buf,command);

        char *content = NULL;
        int ilen = make_http_content(buf,&command);
        if(ilen > 0)
        {
            send(st,content,ilen,0);
            free(content);
        }
    }
    close(st);
    return NULL;
}