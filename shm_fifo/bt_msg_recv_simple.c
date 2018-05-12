#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include<unistd.h>
#include "app_fifo.h"

static app_fifo_t* bt_fifo;

void printfhex(unsigned char *buf, unsigned int len)
{
	int i;
	printf("\r\nmain process recv data len is :%d\r\n", len);
	for(i=0;i<len;i++){
		printf("%02x ", buf[i]);
	}
	printf("\r\n");
}

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
 
int main (void) {
	bluetooth_proxy_init();
	char data[1024];	
	while(1){
		//main process recv data from python process		
		uint32_t data_len = fifo_length(bt_fifo);
		uint32_t bt_fifo_status=app_fifo_get_multiple(bt_fifo, data, data_len);
		if(bt_fifo_status==SUCCESS){
			printfhex(data, data_len);
		}
		sleep(2);
	}
}
