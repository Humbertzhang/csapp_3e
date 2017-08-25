int readcnt;
sem_t mutex,w;
void reader()
{
    while(1){
        P(&mutex);
        readcnt += 1;
        if(readcnt == 1)
            P(&w);
        V(&mutex);

        /*Do read thing*/

        P(&mutex);
        readcnt -= 1;
        if(readcnt == 0)
            V(&w);
        V(&mutex);
    }
}

void writer()
{
    while(1){
        P(&w);
        /*Do writer thing.*/
        V(&w);
        reader();   /*Is that right..?*/
    }
}