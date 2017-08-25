sem_t mutex = 1;
void reader()
{
    while(1){
        P(&mutex);
        /*Do some thing*/
        V(&mutex);
    }
}

void writer()
{
    while(1)
    {
        P(&mutex);
        /*Do some thing*/
        V(&mutex); 
    }
}