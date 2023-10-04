#include "shm_func.h"

const size_t MEM_SIZE_NUMBERS = sizeof(unsigned long) * 10;
const size_t MEM_SIZE_FLAGS = sizeof(char) * 11;

key_t get_file_key(char* key_string){
    key_t key = ftok(key_string, 0);
    if(key == -1){
        printf("Error reading");
        printf(" %s ", key_string);
        printf("key file\n");
        return(0);
    }
    return(key);
}

unsigned long* create_shared_number_ptr(key_t key){
    int block_id = shmget(key, MEM_SIZE_NUMBERS, 0666 | IPC_CREAT);
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

char* create_shared_flag_ptr(key_t key){
    int block_id = shmget(key, MEM_SIZE_FLAGS, 0666 | IPC_CREAT);
    if(block_id == -1){
        printf("Error creating block ID\n");
        return(NULL);
    }

    char* sh_ptr = (char*)shmat(block_id, NULL, 0);
    if(sh_ptr == (char*)NULL){
        printf("Error binding block to address space\n");
        return((char*)NULL);
    }

    return(sh_ptr);
}


