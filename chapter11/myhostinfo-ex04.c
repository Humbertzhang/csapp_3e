#include "csapp.c"

int main(int argc,char ** argv)
{
    struct addrinfo   *p, *listp, hints;
    struct sockaddr_in *sockp; //. 
    char buf[MAXLINE];
    int rc,sym;
    
    if(argc != 2){
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    /* Get a list of addrinfo records */
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;  
    
    if ((rc = getaddrinfo(argv[1],NULL,&hints,&listp)) != 0){           //因为只想要ip address ,所以service 端用的NULL
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }

    
    /* Walk the list and display each IP address */
    for(p = listp; p; p = p->ai_next ) {
        sockp = (struct sockaddr_in *)p->ai_addr;
        Inet_ntop(AF_INET,&(sockp->sin_addr),buf,MAXLINE);              //勘误:这里书上练习题部分写成了 inet_pton
        printf("%s\n",buf);
    }

    /* Clean Up */
    Freeaddrinfo(listp);
    
    exit(0);
}
