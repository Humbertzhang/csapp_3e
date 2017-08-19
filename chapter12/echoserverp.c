#include "csapp.c"

void echo(int connfd);

void sigchild_handler(int sig)
{
    while(waitpid(-1,0,WNOHANG)>0);
    return;
}

int main(int argc, char ** argv)
{
    int listenfd,connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if(argc != 2){
        fprintf(stderr,"usage %s <port>\n",argv[0]);
        exit(0);        
    }

    Signal(SIGCHLD,sigchild_handler);
    listenfd = Open_listenfd(argv[1]);
    
    while(1){
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd,(SA *)&clientaddr,&clientlen);
        if(Fork() == 0){
            Close(listenfd); /* Child close listening socket */
            echo(connfd);    /* Child services client */
            Close(connfd);   /* Child closes connection with client */
            exit(0);
        }
        Close(connfd);       /* Parent close connected socket */
    }
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio,connfd);
    while((n = Rio_readlineb(&rio,buf,MAXLINE)) != 0){
        printf("server received %d bytes\n", (int)n );
        Rio_writen(connfd,buf,n);
    }
    
}