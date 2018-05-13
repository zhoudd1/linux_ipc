#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() { 
    // 1. 获取 SHM
    int shm_id = shmget(13, 2048, IPC_CREAT | 0666);
    
    if (shm_id != -1) {
        // 2. 映射 SHM
        void* shm = shmat(shm_id, NULL, 0);
        if (shm != (void*)-1) {
            // 3. 读取 SHM
            char str[50] = { 0 };
            memcpy(str, shm, strlen("I'm share memory"));
            printf("shm = %s\n", (char *)shm); 
            // 4. 关闭 SHM
            shmdt(shm);
        } else {
            perror("shmat:");
        }
    } else {
        perror("shmget:");
    }
    if (0 == shmctl(shm_id, IPC_RMID))
        printf("delete shm success.\n");
    return 0;
}
