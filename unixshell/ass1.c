#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<signal.h>
#include "macros.h"
#include "ass1Functionality.h"
int readLine(char *p, int n);
int readToken(const char *,char *);

int main(void)
{
    struct sigaction s; 
    s.sa_sigaction=childHandler;
    s.sa_flags=SA_SIGINFO|SA_NOCLDSTOP;
    sigaction(SIGCHLD,&s,NULL);
    char cmdline[LINE_LENGTH+1];
    char cmd[MAX_LENGTH+1];
    int num_tokens = 0,tptr=0;
    do
    {
        printf("shell379:\t");
        num_tokens=readLine(cmdline,LINE_LENGTH);
        if(num_tokens==0)
        {
            printf("Enter Valid Command (Not Empty)\n");
            continue;
        }
        tptr=0;
        tptr += readToken(cmdline,cmd);
        char args[num_tokens-1][MAX_LENGTH+1]; //num args = num tokens -1
        char *argptrs[num_tokens+1]; //num tokens +1 because ended with null
        argptrs[0]=cmd;
        for(int i=0;i<num_tokens-1;i++)
        {
           tptr += readToken(cmdline+tptr,args[i]);
           argptrs[i+1]=args[i];
        }
        argptrs[num_tokens]=NULL;
        doSmthg(num_tokens+1,cmd,argptrs,cmdline);
    } while (true);
    

}

int readLine(char *p, int n) // length of p must be n+1 atleast
{
    int i = 0;
    int numTokens=0;
    while(i<n)
    {
        p[i]=getchar();
        if(p[i]=='\n')
            break;
        if((i==0&&(p[i]==' '||p[i]=='\t'))||(p[i]==' '&&p[i-1]==' '))
            continue; //skips leading white space and multiple spaces
        if(p[i]==' ')
            numTokens++;
        i++;
    }
    if(i!=0&&p[i-1]!=' ')
        numTokens++;
    p[i]='\0';
    return numTokens; // number of tokens read
}
int readToken(const char *s, char *a)
{
    int x=0;
    while(s[x]!=' '&&s[x]!='\0')
    {
        a[x]=s[x];
        x++;
    }
    a[x]='\0';
    return x+1;
}
