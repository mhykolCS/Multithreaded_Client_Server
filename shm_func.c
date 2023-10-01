#include "shm_func.h"


key_t get_file_key(){
    key_t key = ftok("sh_mem.key", 0);
    if(key == -1){
        printf("Error reading 'sh_mem.key' file\n");
        return(0);
    }
    return(key);
}

char* create_shared_ptr(key_t key){
    int block_id = shmget(key, BLOCK_SIZE, 0666 | IPC_CREAT);
    if(block_id == -1){
        printf("Error creating block ID\n");
        return((char *)0);
    }

    char* sh_ptr = shmat(block_id, NULL, 0);
    if(sh_ptr == (char *)-1){
        printf("Error binding block to address space\n");
        return((char *)0);
    }

    return(sh_ptr);
}