#include <time.h>
#include<stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>


int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int r; 
    int fd = open("./test.txt",O_WRONLY|O_CREAT,S_IRWXU);
    for(int i =0;i<10000;i++)
    {
        r=rand()%10;
        if(r<3)
        {
            dprintf(fd,"S<%d>\n",r*10);
        }
        else
            dprintf(fd,"T<%d>\n",r*10);


    }
}