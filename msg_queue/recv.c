#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
 
int main (void) {
 
        key_t ipckey;
        int mq_id;
        struct { long type; char text[100]; } mymsg;
        int received;
 
        /* Generate the ipc key */
        ipckey = ftok("/tmp/foo", 42);
        printf("My key is %d\n", ipckey);
 
        /* Set up the message queue */
        mq_id = msgget(ipckey, 0);
        printf("Message identifier is %d\n", mq_id);
 
        received = msgrcv(mq_id, &mymsg, sizeof(mymsg), 0, 0);
 
        printf("%s (%d)\n", mymsg.text, received);
}
