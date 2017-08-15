#include "csapp.c"
int main(int argc, char ** argv)
{
    int n;
    rio_t rio;
    char buf[MAXBUF];
    if(argv[1] != NULL){
        int fd1 = Open(argv[1],O_RDONLY,0);
        Rio_readinitb(&rio,fd1);
    }
    else{
        Rio_readinitb(&rio,STDIN_FILENO);
    }

    while((n = Rio_readnb(&rio,buf,MAXBUF))!=0)
        Rio_writen(STDOUT_FILENO,buf,n);
    
}
