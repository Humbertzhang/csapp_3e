#include "csapp.c"

int main()
{
    int fd;
    char c;

    fd = Open("foobar.txt",O_RDONLY,0);
    if(Fork() == 0){
        Read(fd,&c,1);
        exit(0);
    }
    
    if(Fork() == 0){
        Read(fd,&c,1);
        exit(0);
    }

    if(Fork() == 0){
        Read(fd,&c,1);
        exit(0);
    }
    if(Fork() == 0){
        Read(fd,&c,1);
        exit(0);
    }
    Wait(NULL);
    Read(fd,&c,1);
    printf("c = %c\n",c);
    exit(0);
}
