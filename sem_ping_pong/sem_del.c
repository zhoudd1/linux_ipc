#include <semaphore.h>  
    #include <stdio.h>  
      
    void sem_del(char *name)  
    {  
        int ret;  
      
        ret = sem_unlink(name);  
        if(ret < 0)  
        {  
            perror("sem_unlink");  
        }  
    }  
      
    int main(int argc, char **argv)  
    {  
        sem_del("sem_print1"); //删除信号量文件sem_print1  
        sem_del("sem_print2"); //删除信号量文件sem_print2  
          
        return 0;  
    }
