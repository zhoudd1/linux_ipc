#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define err_sys(msg) \
    do { perror(msg); exit(-1); } while(0)
#define err_exit(msg) \
    do { fprintf(stderr, msg); exit(-1); } while(0)

void *r1(void *arg)
{
    sem_t* sems = (sem_t *)arg;
    static int cnt = 10;

    while(cnt--)
    {
        sem_wait(sems);
        printf("I am in r1. I get the sems.\n");
    }
}

void *r2(void *arg)
{
    sem_t* sems = (sem_t *)arg;
    static int cnt = 10;

    while(cnt--)
    {
        printf("I am in r2. I send the sems\n");
        sem_post(sems);
        sleep(1);
    }
}

int main(void)
{
    sem_t sems;
    pthread_t t1, t2;
    
    printf("sems size: %d\n", sizeof(sems));
    /* sem_init()第二个参数为0表示这个信号量是当前进程的局部信号量，否则该信号
     * 就可以在多个进程之间共享 */
    if(sem_init(&sems, 0, 0) < 0)
        err_sys("sem_init error");
    pthread_create(&t1, NULL, r1, &sems);
    pthread_create(&t2, NULL, r2, &sems);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&sems);

    return 0;
}
