/* Exercise 11.6 */

#include "csapp.c"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri,char *filename,char *cgiargs);
void server_static(int fd,char *filename,int filesize);
void get_filetype(char *filename,char *filetype);
void server_dynamic(int fd,char *filename,char *cgiargs);
void clienterror(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg);

int main(int argc,char ** argv)
{
    int listenfd,connfd;
    char hostname[MAXLINE],port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* http://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/socket.h.html */

    /* Check command line args  */
    if(argc!=2){
        fprintf(stderr,"usage:%s <port>\n",argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);   /* argv[1] 来确定监听哪一个端口 */
    
    /* 无限服务器循环 */
    while(1){ 
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd,(SA *)&clientaddr,&clientlen);/* 等待来自客户端的链接请求　SA is socket address*/
        Getnameinfo((SA *)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        printf("Accepted connection from (%s , %s)\n",hostname,port);
        doit(connfd);
        Close(connfd);
    }
}

void doit(int fd)
{
    int is_static;
    struct stat sbuf;  /* struct stat in csapp:3e page632 */
    char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    char filename[MAXLINE],cgiargs[MAXLINE];   /* CGI 同用网管接口，cgiargs CGI的参数 */
    rio_t rio;
    
    /* Read request line and headers */
    Rio_readinitb(&rio,fd);
    Rio_readlineb(&rio,buf,MAXLINE);
    printf("Request headers:\n");
    printf("%s",buf);
    sscanf(buf,"%s %s %s",method,uri,version);
    
    
    /*
    *
    *Home Work 11.6
    *
    */

    char msg1[100] = "\n<----Request Line Start---->\n";
    Rio_writen(fd,msg1,100);
    Rio_writen(fd,buf,MAXLINE);
    char msg2[100] = "<----Request Line End---->\n";
    Rio_writen(fd,msg2,100);


    if(strcasecmp(method,"GET")){  /* Strcasecmp(): A Thread Safe strcmp() http://man7.org/linux/man-pages/man3/strcasecmp.3.html */
        clienterror(fd,method,"501","Not implemented",
                    "Tiny dose not implement this method"); /*只有与"GET"相同时为0,才不会执行if下的语句*/
        return;
    }
    read_requesthdrs(&rio);
    
    /* Parse URI from GET request */
    is_static = parse_uri(uri,filename,cgiargs);
    if(stat(filename,&sbuf) < 0){
        clienterror(fd,filename,"404","Not Found",
                    "Tiny coundn't found this file");
        return;
    }

    /* Server static content */
    if (is_static){ 
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)){    /* S_ISREG 判断是不是普通文件　csapp page632 S_IRUSR csapp page 625 */
            clienterror(fd,filename,"403", "Forbidden",
                        "Tiny couldn't read this file");
            return;
        }
        server_static(fd,filename,sbuf.st_size);
    }

    /* Server dynamic content */
    else{
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)){
            clienterror(fd,filename,"403", "Forbidden",
                        "Tiny couldn't read this file");
            return;
        }
        server_dynamic(fd,filename,cgiargs);
    }
}

void clienterror(int fd,char *cause,char *errnum, char *shortmsg,char *longmsg)
{
    char buf[MAXLINE],body[MAXLINE];

    /* Build HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor = ""ffffff"" >\r\n",body);
    sprintf(body, "%s%s: %s\r\n",body,errnum,shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n",body,longmsg,cause);
    sprintf(body, "%s<hr><em>The Tiny Web Server</em>\r\n",body);

    /* Print the HTTP response */
    sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    Rio_writen(fd,buf,strlen(buf));   /* Have rio_initb() above in doit() function */
    sprintf(buf,"Content-type: text/html\r\n");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Content-length: %d\r\n\r\n",(int)strlen(body));
    Rio_writen(fd,buf,strlen(buf));
    Rio_writen(fd,body,strlen(body));
}

/* Tiny 仅仅读取请求报头并忽略 */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    /*
    *
    *Home Work 11.6
    *
    */ 
    char buf2[MAXLINE] = "\n<----Request Header start---->\n";
    Rio_writen(rp->rio_fd,buf2,MAXLINE);

    Rio_readlineb(rp,buf,MAXLINE);
    while(strcmp(buf,"\r\n")){  /* 检查到空的请求报头停止 */
        Rio_writen(rp->rio_fd,buf,MAXLINE);

        Rio_readlineb(rp,buf,MAXLINE);
        printf("%s",buf);
    }

    char buf3[MAXLINE] = "\n<----Request Header end---->\n";
    Rio_writen(rp->rio_fd,buf3,MAXLINE);

    return;
}

int parse_uri(char *uri,char *filename,char *cgiargs)
{
    char *ptr;
    
    if(!strstr(uri,"cgi-bin")){ /* Static content */
        strcpy(cgiargs,"");
        strcpy(filename,".");
        strcat(filename,uri);
       
        if(uri[strlen(uri)-1] == '/')
            strcat(filename,"home.html");
        return 1;
    }
    else{
        ptr = index(uri,'?');
        if(ptr){
            strcpy(cgiargs,"");
            *ptr = '\0';
        }
        else
            strcpy(cgiargs,"");
        strcpy(filename,".");
        strcat(filename,uri);
        return 0;
    }
}

void get_filetype(char *filename,char *filetype)
{
    if (strstr(filename, ".html"))
	strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
	strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
	strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
	strcpy(filetype, "image/jpeg");
    else
	strcpy(filetype, "text/plain");
}

void server_static(int fd, char * filename, int filesize)
{
    int srcfd;
    char *srcp,filetype[MAXLINE],buf[MAXBUF];

    /* send response headers to client */
    get_filetype(filename,filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));       //line:netp:servestatic:endserve
    printf("Response headers:\n");
    printf("%s", buf);

    /* Send response body to client */
    srcfd = Open(filename,O_RDONLY,0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    Rio_writen(fd,srcp,filesize);
    Munmap(srcp,filesize);
}

void server_dynamic(int fd,char *filename,char *cgiargs)
{
    char buf[MAXLINE],*emptylist[] = {NULL};

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if(Fork() == 0){
        setenv("QUERY_STRING",cgiargs,1);
        Dup2(fd,STDOUT_FILENO);
        Execve(filename,emptylist,environ); /*Run CGI program */
    }
    Wait(NULL); /* Parent waits for and reaps child */
}   