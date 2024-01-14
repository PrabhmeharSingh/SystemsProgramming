#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include<pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include<semaphore.h>
#include<assert.h>
#include<sys/stat.h>
#include"tands.h"
int fd = -1;
struct buf
{
    int start;
    int end;
    int size;
    int *arr;
}b={0,0,0,NULL};
struct status
{
    int works;
    int asks;
    int receives;
    int completes;
    int sleeps;
    int q;
    int *nc;
}s={0,0,0,0,0,0,NULL};
sem_t bufFull, bufEmpty, conLock, finished, askLock, rLock, cLock;
void Sem_wait(sem_t *s) {
    int rc = sem_wait(s);
    assert(rc == 0);
}
void Sem_post(sem_t *s) {
    int rc = sem_post(s);
    assert(rc == 0);
}
void* consumer(void *a) // may return number of completes by this thread
{
    int x,num;
    while(1) // might be problematic
    {
        //ask
        Sem_wait(&askLock);
        s.asks++;
        Sem_post(&askLock);
        dprintf(fd,"ID = %lu\t\tAsk\t\t\n",pthread_self());
        Sem_wait(&bufEmpty); //if there is nothing to take after this and queue is empty then 
        Sem_wait(&conLock); //
        x=*(b.arr+b.start);
        b.start=(b.start+1)%b.size;
        Sem_post(&conLock);
        Sem_post(&bufFull);
        Sem_wait(&rLock);
        s.receives++;
        Sem_post(&rLock);
        dprintf(fd,"ID = %d\tQ=%d\tReceive\t\tn=%d\n",gettid()-getpid(),s.q,x);
        Trans(x);
        Sem_wait(&cLock);
        s.completes++;
        Sem_post(&cLock);
        dprintf(fd,"ID = %lu\t\tComplete\t\tn=%d\n",pthread_self(),x);
        num++;
        Sem_post(&finished);
    }


}
void producer(void)
{
    int r = 0,n=0;
    do
    {
       r = scanf(" T<%d>",&n); 
       if(r>0)
       {
            s.works++;
            s.q=(b.end-b.start)%b.size;
            if(s.q<0)
            {
                s.q+=b.size;

            }
            dprintf(fd,"ID = %lu\tQ=%d\tWork\t\tn=%d\n",pthread_self(),s.q,n);
            //Transaction Acknowledging Code
            Sem_wait(&bufFull);
            *(b.arr+b.end)=n;
            b.end = (b.end+1)%b.size;
            sem_post(&bufEmpty);
       }
       else
       {
            r = scanf(" S<%d>",&n);
            if(r>0)
            {
                s.sleeps++;
                dprintf(fd,"ID = %lu\t\tSleep\t\tn=%d\n",pthread_self(),n);
                Sleep(n);
            }
       }
    } while (r>0);
    do{
        Sem_wait(&finished);
    }while(b.start!=b.end);
    
}
int main(int argc,char *argv[])
{
    if(argc<2)
        return 1;
    int numThreads=0,id=0;
    char fileName[20];
    sscanf(argv[1],"%d",&numThreads);
    if(argc>2)
    {
        sscanf(argv[2],"%d",&id);
    }
    if(id==0)
    {
        strcpy(fileName,"./prodcon.log");
    }
    else
    {
        sprintf(fileName,"./prodcon.%d.log",id);
    }
    fd = open(fileName,O_WRONLY|O_CREAT,S_IRWXU);
    b.arr=malloc(2*numThreads*sizeof(int));
    b.size = 2*numThreads;
    s.nc = malloc(numThreads*sizeof(int));
    sem_init(&bufFull,0,2*numThreads);
    sem_init(&bufEmpty,0,0);
    sem_init(&finished,0,0);
    sem_init(&conLock,0,1);
    sem_init(&askLock,0,1);
    sem_init(&rLock,0,1);
    sem_init(&cLock,0,1);
    pthread_t p[numThreads];
    for(int i=0;i<numThreads;i++)
    {
        if(pthread_create(&p[i],NULL,consumer,NULL)!=0)
        {
            perror("Thread Creation Error");
            exit(1);
        }
    }
    producer();
    
    
    for(int i=0;i<numThreads;i++)
    {
        pthread_cancel(p[i]);
        pthread_join(p[i],NULL);
    }
    free(b.arr);
    free(s.nc);
}