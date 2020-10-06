#include "pub.h"

char buff[2024];
void save_log(char * buff)
{
    FILE * fp = fopen("log.txt","a+");
    fputs(buff,fp);
    fclose(fp);
}
void setdaemon()
{
    pid_t pid,sid;
    pid = fork();
    umask(0);
    if(pid < 0)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"error set daemon");
        save_log(buff);
        exit(EXIT_FAILURE);
    }
    else if(pid != 0)
    {
        exit(0);
    }
    sid = setsid();
    if(sid<0)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"error set daemon");
        save_log(buff);
    }
    umask(0);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}


const char * get_filetype(const char *filename)
{
    char sExt[32];
    const char * index = filename;
    while(*index)
    {
        if(*index == '.')
        {
            index++;
            strncpy(sExt,index,sizeof(sExt));
        }
    }
    if (strncmp(sExt, "bmp", 3) == 0)
		return "image/bmp";

	if (strncmp(sExt, "gif", 3) == 0)
		return "image/gif";

	if (strncmp(sExt, "ico", 3) == 0)
		return "image/x-icon";

	if (strncmp(sExt, "jpg", 3) == 0)
		return "image/jpeg";

	if (strncmp(sExt, "avi", 3) == 0)
		return "video/avi";

	if (strncmp(sExt, "css", 3) == 0)
		return "text/css";

	if (strncmp(sExt, "dll", 3) == 0)
		return "application/x-msdownload";

	if (strncmp(sExt, "exe", 3) == 0)
		return "application/x-msdownload";

	if (strncmp(sExt, "dtd", 3) == 0)
		return "text/xml";

	if (strncmp(sExt, "mp3", 3) == 0)
		return "audio/mp3";

	if (strncmp(sExt, "mpg", 3) == 0)
		return "video/mpg";

	if (strncmp(sExt, "png", 3) == 0)
		return "image/png";

	if (strncmp(sExt, "ppt", 3) == 0)
		return "application/vnd.ms-powerpoint";

	if (strncmp(sExt, "xls", 3) == 0)
		return "application/vnd.ms-excel";

	if (strncmp(sExt, "doc", 3) == 0)
		return "application/msword";

	if (strncmp(sExt, "mp4", 3) == 0)
		return "video/mpeg4";

	if (strncmp(sExt, "ppt", 3) == 0)
		return "application/x-ppt";

	if (strncmp(sExt, "wma", 3) == 0)
		return "audio/x-ms-wma";

	if (strncmp(sExt, "wmv", 3) == 0)
		return "video/x-ms-wmv";

	return "text/html";
}

int sock_create(int port)
{
    int st = socket(AF_INET,SOCK_STREAM,0);
    int on = 1;
    if(st == -1)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"%s,%d:socket error %s\n",__FILE__,__LINE__,strerror(errno));
        save_log(buff);
        return -1;
    }
    if(setsockopt(st,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==-1)
    {
        memset(buff,0,sizeof(buff));
		sprintf(buff,"setsockopt failed %s\n", strerror(errno));
		save_log(buff);
		return -1;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(st,(struct sockaddr *)&addr,sizeof(addr))==-1)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"bind error");
        save_log(buff);
        return -1;
    }

    if(listen(st,100)==-1)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"listen error");
        save_log(buff);
        return -1;
    }
    save_log("start server success!\n");
    return st;;
}

int sock_accept(int st)
{
    int client_st ;
    struct sockaddr_in client_sockaddr;
	socklen_t len = sizeof(client_sockaddr);

    pthread_t thrd_t;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //初始化线程为可分离的
	memset(&client_sockaddr, 0, sizeof(client_sockaddr));

    while(1)
    {
        client_st = accept(st,(struct sockaddr_in *)&client_sockaddr,&len);
        //client_st = accept(st, (struct sockaddr *) &client_sockaddr, &len);
		if (client_st == -1)
		{
			memset(buff,0,sizeof(buff));
			sprintf(buff,"%s,%d:accept failture %s \n", __FILE__, __LINE__,
					strerror(errno));
			save_log(buff);
			return 0;
		}
        else
        {
            int *tmp = (int *) malloc(sizeof(int));
			*tmp = client_st;
			pthread_create(&thrd_t, &attr, http_thread, tmp);
        }
    }
    pthread_destory(&attr);//释放资源
}

int get_file_content(const char * filename,char * content[])
{
    int file_length = 0;
    FILE * fp = NULL;

    if(filename == NULL)
    {
        return file_length;
    }
    fp = fopen(filename,"rb");
    
    if(fp == NULL)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"file name: %s,%s,%d\n",filename,__FILE__,__LINE__);
        save_log(buff);
        return file_length;
    }
    fseek(fp,0,SEEK_END);
    file_length = ftell(fp);
    * content = (char *)malloc(file_length);
    if(*content == NULL)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"malloc error: %s,%s,%d",filename,__FILE__,__LINE__);
        save_log(buff);
        return 0;
    }
    fread(*content,file_length,1,fp);
    fclose(fp);
    return file_length;
}