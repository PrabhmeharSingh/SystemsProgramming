#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "macros.h"
#include "processList.h"
typedef struct 
{
    pid_t processId;
    char status;
    char fullcmd[LINE_LENGTH+1];
} process;

process *processList[MAX_PT_ENTRIES]; 
int numRunningProcesses = 0;
int addProcess(pid_t id,const char cmd[]) //returns index of the process just created
{
    if(numRunningProcesses>=MAX_PT_ENTRIES)
        return -1;
    processList[numRunningProcesses] = (process *)malloc(sizeof(process));
    *(processList[numRunningProcesses])=(process){id,'R'};
    strcpy((*(processList[numRunningProcesses])).fullcmd,cmd);
    return numRunningProcesses++;
}
int findProcess(pid_t id) //returns index of the process just created
{
    for(int i=0;i<numRunningProcesses;i++)
    {
        if((*(processList[i])).processId==id)
            return i;
    }
    return -1; //could not find process
}
int removeProcess(pid_t id)
{
    int a =findProcess(id);
    if(a==-1)
        return -1; 
    free(processList[a]);
    numRunningProcesses--;
    for(int i=a;i<numRunningProcesses;i++)
    {
        processList[i]=processList[i+1];
    }
    processList[numRunningProcesses]=NULL;
    return 0;
}
char changeProcessStatus(pid_t id, char n)
{
    int a =findProcess(id);
    if(a==-1)
        return '\0';
    char x =(*(processList[a])).status;
    (*(processList[a])).status =n;
    return x;
}

int getNumRunningProcesses()
{
   
    return numRunningProcesses;
    
}
