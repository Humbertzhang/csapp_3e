#include "csapp.c"

int main(int argc , char **argv)
{
    struct stat stat;
    char *type ,*readok;
    int fd,fd1,fd2;
    /*
    DIR *stream;
    struct dirent *dep;

    fd1 = Open("foo.txt",O_RDONLY,0);   //regular , argv[1] = 3
    
    stream = Opendir("/");                 // directory argv[1] = 4
    dep = Readdir(stream);
    fd2 = dep -> d_ino;
    */
    fd = atoi(argv[1]);

    Fstat(fd,&stat);

    if(S_ISREG(stat.st_mode))
        type = "regular";
    else if(S_ISDIR(stat.st_mode))
        type = "directory";
    else
        type = "other";

    if((stat.st_mode & S_IRUSR))
        readok = "yes";
    else
        readok = "no";

    printf("type : %s, read : %s\n",type,readok);
    exit(0);
}
