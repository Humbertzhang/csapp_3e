#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char c[100]={'\0'};
    int add[4];
    int start = 0, count = 0, j = 0;
    
    printf("Input the ipaddress like 127.0.0.1:");
    scanf("%s", c);
    
    printf("0x");
    
    for (int i = 0; i < 100; i++) {
        if (c[i] == '.' || c[i] == '\0') {
            char temp[10] = {'\0'};
            memcpy(temp, &c[start], count);
            add[j] = atoi(temp);
            printf("%02x", add[j++]);
            start = i + 1;
            count = 0;

            if(j==4)
                break;
        }

        else
            count++;
    }
    printf("\n");
    return 0;
}

