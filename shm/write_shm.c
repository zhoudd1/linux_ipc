#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

int main() {
    // 1. 创建 SHM
    int shm_id = shmget(13, 2048, IPC_CREAT | 0666);
    if (shm_id != -1) {
        // 2. 映射 SHM
        void* shm = shmat(shm_id, NULL, 0);
        if (shm != (void*)-1) {
            // 3. 写 SHM
            char str[] = "I'm share memory";
            memcpy(shm, str, strlen(str) + 1);
            // 4. 关闭 SHM
            shmdt(shm);
        } else {
            perror("shmat:");
        }
    } else {
        perror("shmget:");
    }
    return 0;
}
