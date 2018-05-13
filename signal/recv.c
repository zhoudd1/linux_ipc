#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
void new_op(int,siginfo_t*,void*);
int main(int argc,char**argv)
{
    struct sigaction act;
    int sig;
    pid_t pid;      
     
    pid=getpid();
    sig=atoi(argv[1]);  
     
    sigemptyset(&act.sa_mask);
    act.sa_sigaction=new_op;
    act.sa_flags=SA_SIGINFO;
    if(sigaction(sig,&act,NULL)<0)
    {
        printf("install sigal error\n");
    }
    while(1)
    {
        sleep(2);
        printf("pid %d wait for the signal\n",pid);
    }
}
void new_op(int signum,siginfo_t *info,void *myact)
{
    printf("the int value is %d \n",info->si_int);
}
