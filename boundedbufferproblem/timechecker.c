#include<stdio.h>
#include<time.h>
int main()
{
    struct timespec t,e;
    clock_gettime(CLOCK_REALTIME,&t);
    printf("Start time = %ld",t.tv_nsec);
    int x=0;
    for(int i=0;i<1000000;i++)
    {
        x+=i*2;
        x=(x-5)/2;
    }
    clock_gettime(CLOCK_REALTIME,&e);
    printf("time passed = %ld",e.tv_nsec-t.tv_nsec);

}