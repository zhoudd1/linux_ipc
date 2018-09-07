#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_LEN     100
#define MSG_KEY    1234

//msg_queue
typedef struct 
{
    long int type; 
    char data[MSG_LEN];   
}msg_t;

static int msgid;

//msg queue init
int msg_queue_creat(key_t key)
{   
	msgid=msgget(key,0666 | IPC_CREAT);
	if(msgid==-1)
	{
	    fprintf(stderr,"msgget failed with error: %d\n",errno);		
	}
    return 0;
}

int main()
{
    msg_queue_creat(MSG_KEY);
 
    while(1) {  
        msg_t msg={1,"hello,world"};
        if(msgsnd(msgid, &msg, sizeof(msg_t), IPC_NOWAIT) == -1)
	    {
	        //fprintf(stderr,"msgsnd failed with error: %d\n",errno);
	    }
        else printf("send a message\n");        
        sleep(2);
    }
}
