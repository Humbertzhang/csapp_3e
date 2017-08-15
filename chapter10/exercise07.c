#include "csapp.c"
#define MAXBUF2 10
int main()
{
    int n;
    rio_t rio;
    char buf[MAXBUF2];

    Rio_readinitb(&rio,STDIN_FILENO);
    while((n = Rio_readnb(&rio,buf,MAXBUF2))!=0)
        Rio_writen(STDOUT_FILENO,buf,n);
    
}
