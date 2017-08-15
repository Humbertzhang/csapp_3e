#include <stdio.h>
#include <stdlib.h>


int main(int argc,char ** argv)
{
    unsigned int address;
    int add[4];
    printf("Input hex argument:");
    scanf("%x",&address);
    for(int i=0;i<4;i++){
        add[i] = ((address >> 2*(3-i)*4) & (0x000000ff));
        printf("%d",add[i]);
        if(i!=3)
            printf(".");
        else
            printf(".\n");
    }
}
