#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define BLOCK_SIZE 352

int main(int argc, char** argv[]){

    key_t key = ftok("sh_mem.key", 0);
    if(key == -1){
        printf("Error reading 'sh_mem.key' file\n");
        return(0);
    }

    int block_id = shmget(key, BLOCK_SIZE, 0666 | IPC_CREAT);
    if(block_id == -1){
        printf("Error creating block ID\n");
        return(0);
    }

    char* sh_ptr = shmat(block_id, NULL, 0);
    if(sh_ptr == (char *)-1){
        printf("Error binding block to address space\n");
        return(0);
    }

    printf("%s\n", sh_ptr);

    return(0);
}