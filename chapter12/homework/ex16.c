#include "../csapp.c"
#include <time.h>
/*使用clock()/CLOCKS_PER_SEC来计算一个进程自身的运行时间*/
/*单位是秒，需要一定的数量级才能跟单线程的比较出效果*/

void * thread(void * vargp);

int main(int argc,char ** argv)
{
    int i,n;
    pthread_t tid;
    int start,end,rst;

    if(argc != 2){
        printf("Usage:./a.out number\n");
        exit(1);
    }
    start = clock();
    n = atoi(argv[1]);
    for(i=0;i<n;i++){
        Pthread_create(&tid,NULL,thread,NULL);
        Pthread_join(tid,NULL);
    }

    end = clock();
    rst = end - start;
    printf("time:%ld\n",rst/CLOCKS_PER_SEC);
    return 0;
}

void * thread(void * vargp)
{
    printf("Hello,World!\n");
    return NULL;
}