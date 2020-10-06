.SUFFIXES:.c .o
CC = gcc
SRCS = httpServer.c\
    pub.c\
    thread_pool.c
OBJS = $(SRCS:.c=.o)
EXEC = httpServer
start:$(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -lpthread
.c.o:
	$(CC) -o $@ -c $<
clean:
	rm -f $(OBJS)

