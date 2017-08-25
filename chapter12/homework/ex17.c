#include "../csapp.c"
void * func(void * vargp);
int main()
{
    pthread_t tid;
    Pthread_create(&tid,NULL,func,NULL);
    Pthread_exit(&tid);
}

void * func(void * vargp)
{
    Sleep(1);
    printf("Hello,World!\n");
    return NULL;
}