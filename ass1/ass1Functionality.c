#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <sys/wait.h>
#include <sys/times.h>
#include "macros.h"
#include "processList.h"
#include "ass1Functionality.h"
#define numCommands 7
#define CTPS sysconf(_SC_CLK_TCK)
typedef struct
{
    char cmdName[10];
    int (*func)(int n, char *[n]);
} command;
int myexit(int num, char *args[num]);
int myjobs(int num, char *args[num]);
int mykill(int num, char *args[num]);
int myresume(int num, char *args[num]);
int mysleep(int num, char *args[num]);
int mysuspend(int num, char *args[num]);
int mywait(int num, char *args[num]);
const command availCmds[numCommands] ={
    {"exit",myexit},
    {"jobs",myjobs},
    {"kill",mykill},
    {"resume",myresume},
    {"sleep",mysleep},
    {"suspend",mysuspend},
    {"wait",mywait}
};

void doSmthg(int num,const char cmd[],char *args[],const char fullcmd[]) // num is number of arguments including null
{
    for(int i=0;i<numCommands;i++)
    {
        if(strcmp(cmd,availCmds[i].cmdName)==0)
        {
            availCmds[i].func(num,args);
            return;
        }
    }
    int ampcmp = strcmp(args[num-2],"&");
    pid_t id = fork();
    if(id<0)
    {
        perror("Cannot Fork");
        return;
    }
    else if(id == 0)
    {
        for(int i=1;i<num-1;i++)
        {
            if(*(args[i]) == '>')
            {
                int fd = open(args[i]+1,O_WRONLY|O_CREAT,S_IRWXU);
                dup2(fd,STDOUT_FILENO);
                close(fd);
                args[i]=NULL;
            }
            else if(*(args[i]) == '<')
            {
                int fd = open(args[i]+1,O_RDONLY);
                dup2(fd,STDIN_FILENO);
                close(fd);
                args[i]=NULL;
            }
        }
        if(ampcmp == 0)
            args[num-2]=NULL;
        if(execve(args[0],args,(char *[]){NULL})==-1 )
        {
            perror("Cannot Exec");
            _exit(1);
        }
    }
    else{
        //parent code
        addProcess(id,fullcmd);
        if(ampcmp!=0)
            waitpid(id,NULL,0); 
        return;
    }
    return;
    

}
int myexit(int num, char *args[num]){
    int status=0;
    pid_t id;
    while((id=waitpid(-1,&status,WUNTRACED))>0)
    {
        if(WIFSTOPPED(status))
        {
            char pid[10];
            sprintf(pid,"%d",id);
            myresume(3,(char *[]){NULL,pid,NULL}); //resuming all stopped processes
        }

    }
    printf("Resources Used\n");
    struct tms *b = (struct tms *)malloc(sizeof(struct tms));
    times(b);
    printf("User Time\t%ld seconds\n",(b->tms_cutime)/CTPS);
    printf("System Time\t%ld seconds\n",(b->tms_cstime)/CTPS);
    free(b);
    _exit(0);

}
int myjobs(int num, char *args[num]){
    printf("Running Processes:\n");
    int fds[2];
    pipe(fds);
    pid_t pid = fork();
    if(pid<0)
    {
        perror("Jobs Failed");
        return -1;
    }
    else if(pid == 0)
    {
        close(fds[0]);
        dup2(fds[1],STDOUT_FILENO);
        close(fds[1]);
        char ppid[10];
        sprintf(ppid,"%d",getppid());
        if(execve("/usr/bin/ps",(char *[]){"/usr/bin/ps","--ppid",ppid,"-o","pid,s,times=SEC,args",NULL},(char *[]){NULL})==-1 )
        {
            perror("Cannot Exec");
            _exit(1);
        }
        
    }
    else{
        addProcess(pid,"");
        close(fds[1]);
        char buf[80];
        int n; 
        while ((n=read(fds[0],buf,80))>0)
        {
            printf("%.*s",n,buf);            
        }
        waitpid(pid,NULL,0);
        close(fds[0]);
        
        
    }
    printf("Processes =\t%d active (PS HAS FINISHED BEFORE THIS)\n",getNumRunningProcesses());
    printf("Completed Processes\n");
    struct tms *b = (struct tms *)malloc(sizeof(struct tms));
    times(b);
    printf("User Time\t%ld seconds\n",(b->tms_cutime)/CTPS);
    printf("System Time\t%ld seconds\n",(b->tms_cstime)/CTPS);
    free(b);
    return 0;

}
int mykill(int num, char *args[num]){
    if(num<3)
    {
        perror("Specify Process ID With Kill");
        return -1;
    }
    int i=0;
    sscanf(args[1],"%d",&i);
    pid_t id = (pid_t)i;
    if(kill(id,SIGKILL)<0)
    {
        perror("Error (1)");
        return -1;
    }
    if(removeProcess(id)<0)
    {
        perror("No Such Process in PT (2)");
        return -1;
    }
    return 0;
}
int myresume(int num, char *args[num]){
    if(num<3)
    {
        perror("Specify Process ID With Resume");
        return -1;
    }
    int i=0;
    sscanf(args[1],"%d",&i);
    pid_t id = (pid_t)i;
    if(kill(id,SIGCONT)<0)
    {
        perror("Error (1)");
        return -1;
    }
    if((i=findProcess(id))<0)
    {
        perror("No Such Process in PT (2)");
        return -1;
    }
    if(changeProcessStatus(id,'R')=='R')
    {
        printf("Process Already Running\n");
    }
    return 0;
}
int mysleep(int num, char *args[num]){
    if(num<3)
    {
        perror("Specify Time With Sleep");
        return -1;
    }
    int i=0;
    sscanf(args[1],"%d",&i);
    printf("Sleeping for %d seconds\n",i);
    sleep(i);
    return 0;
}
int mysuspend(int num, char *args[num]){
    if(num<3)
    {
        perror("Specify Process ID With Suspend");
        return -1;
    }
    int i=0;
    sscanf(args[1],"%d",&i);
    pid_t id = (pid_t)i;
    if(kill(id,SIGTSTP)<0)
    {
        perror("Error (1)");
        return -1;
    }
    if((i=findProcess(id))<0)
    {
        perror("No Such Process in PT (2)");
        return -1;
    }
    if(changeProcessStatus(id,'S')=='S')
    {
        printf("Process Already Suspended\n");
    }
    return 0;

}
int mywait(int num, char *args[num]){
    if(num<3)
    {
        perror("Specify Process ID With Wait");
        return -1;
    }
    int i=0;
    sscanf(args[1],"%d",&i);
    pid_t id = (pid_t)i;
    return waitpid(id,NULL,0);
}
void childHandler(int n, siginfo_t * info, void *m)
{
    //printf("PID = %d\n",(*info).si_pid);
    removeProcess((*info).si_pid); //if Process has terminated remove it
    waitpid((*info).si_pid,NULL,0); // clears the OS PT entry, otherwise zombie process would be left 
    return;
}