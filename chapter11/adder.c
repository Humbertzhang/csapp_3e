#include "csapp.c"

int main()
{
    char *buf,*p;
    char arg1[MAXLINE],arg2[MAXLINE],content[MAXLINE];
    int n1=0,n2=0;

    /* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL){
        p = strchr(buf,'&');   /* strchr : https://www.tutorialspoint.com/c_standard_library/c_function_strchr.htm */
        *p = '\0';  /* As the p will bring the character '&', so we make the first char of p is '\0' */
        strcpy(arg1,buf); /* 第一个参数为getenv("QUERY_STRING")所得*/ 
        strcpy(arg2,p+1); /* 第二个参数为strchr(buf,'&')所得 */
        n1 = atoi(arg1);  /* atoi会自动在&停止　*/
        n2 = atoi(arg2);
    }

    /* Make response body */
    sprintf(content,"QUERY_STRING=%s",buf);
    sprintf(content,"Welcome to muxixyz.com: ");
    sprintf(content,"%sTHE Internet addition protal.\r\n<p>",content);
    sprintf(content,"%sThe answer is %d + %d = %d\r\n<p>",content,n1,n2,n1+n2);
    sprintf(content,"%sThanks for visiting!\r\n",content);

    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n",(int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s",content);
    fflush(stdout);

    exit(0);
}