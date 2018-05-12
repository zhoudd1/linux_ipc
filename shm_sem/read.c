#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "sem.h"

typedef struct
{
    double lon;
    double lat;
    double bd_lon;
    double bd_lat;
}gps_info_t;

int main(int argc, const char *argv[])
{
    key_t key;
    int shmid;
    gps_info_t *gps = NULL;
    int create_flag = 0;
    int sem_id;

    if ((key = ftok(".", 'a')) < 0)
    {
        perror("failed to get key");
        exit(-1);
    }

    if ((sem_id = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        if (errno == EEXIST)
        {
            if ((sem_id = semget(key, 1, 0666)) < 0)
            {
                perror("failed to semget");
                exit(-1);
            }
        }
    }

    init_sem(sem_id, 0);

    if ((shmid = shmget(key, sizeof(gps_info_t), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        if (errno == EEXIST)
        {
            if ((shmid = shmget(key, sizeof(gps_info_t), 0666)) < 0)
            {
                perror("failed to create share memory");
                exit(-1);
            }
        }
        else
        {
            perror("failed to shmget");
            exit(-1);
        }
    }
    else
        create_flag = 1;

    if ((gps = shmat(shmid, NULL, 0)) == (void *)(-1))
    {
        perror("failed to shmat");
        exit(-1);
    }
    
    while(1)
    {
        sem_p(sem_id);

        printf("recv lon: %f\n", gps->lon);
        printf("recv lat: %f\n", gps->lat);
        sleep(1);

    }

    if (create_flag == 1)
    {
        if (shmdt(gps) < 0)
        {
            perror("failed to shmdt");
            exit(-1);
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("failed to delete share memory");
            exit(-1);
        }

        delete_sem(sem_id);
    }

    return 0;
}
