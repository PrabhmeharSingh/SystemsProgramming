#ifndef PROCESSLIST_H
#define PROCESSLIST_H
#include<sys/types.h>
int addProcess(pid_t id,const char cmd[]);
int findProcess(pid_t id);
int removeProcess(pid_t id);
char changeProcessStatus(pid_t id, char n);
int getNumRunningProcesses();
#endif