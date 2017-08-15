#include "csapp.c"

int main()
{
    int fd1,fd2;
    char c[10];

    fd1 = Open("foobar.txt",O_RDONLY,0);
    fd2 = Open("foobar.txt",O_RDONLY,0);
    Read(fd1,c,1);
    Read(fd2,c,6);
    printf("c = %s\n",c);
    exit(0);
}
