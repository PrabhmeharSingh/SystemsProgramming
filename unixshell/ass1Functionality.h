#ifndef ASS1FUNCTIONALITY_H
#define ASS1FUNCTIONALITY_H
#include<signal.h>
void doSmthg(int num,const char cmd[],char *args[],const char fullcmd[]);
void childHandler(int n, siginfo_t * info, void *m);
#endif