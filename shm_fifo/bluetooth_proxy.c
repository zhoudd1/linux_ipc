#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "app_fifo.h"

static app_fifo_t* bt_fifo;

int bluetooth_proxy_init(void) 
{
	int shmid;

    if ((shmid = shmget(16, 2048, IPC_CREAT | 0666)) < 0)
    {
        if (errno == EEXIST)
        {
            if ((shmid = shmget(16, 2048, IPC_CREAT | 0666)) < 0)
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

    bt_fifo = shmat(shmid, NULL, 0);
	if (bt_fifo == (void*)-1)
    {
        perror("failed to shmat");
        exit(-1);
    }

	app_fifo_init(bt_fifo, FIFO_BUF_LEN);

	return 0;
}

void printfhex(unsigned char *buf, unsigned int len)
{
	int i;
	printf("\r\nproxy send data len is :%d\r\n", len);
	for(i=0;i<len;i++){
		printf("%02x ", buf[i]);
	}
	printf("\r\n");
}

int bluetooth_print(char* str, int len)
{
	printfhex(str, len);

	//python send data to main process share memery
	app_fifo_put_multiple(bt_fifo, str, len);

	return 0;
}


