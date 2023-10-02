#include "shm_func.h"

const size_t MEM_SIZE = sizeof(unsigned long)* 11;

key_t get_file_key(){
    key_t key = ftok("sh_mem.key", 0);
    if(key == -1){
        printf("Error reading 'sh_mem.key' file\n");
        return(0);
    }
    return(key);
}

unsigned long* create_shared_ptr(key_t key){
    int block_id = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);
    if(block_id == -1){
        printf("Error creating block ID\n");
        return(NULL);
    }

    unsigned long* sh_ptr = (unsigned long*)shmat(block_id, NULL, 0);
    if(sh_ptr == (unsigned long*)NULL){
        printf("Error binding block to address space\n");
        return((unsigned long*)NULL);
    }

    return(sh_ptr);
}