#include "shm_func.h"



int main(int argc, char** argv[]){

    // configure share memory file keys
    key_t number_key = get_file_key((char*)"number.key");
    key_t flag_key = get_file_key((char*)"flag.key");
    if(number_key == 0){
        return(0);
    }
    if(flag_key == 0){
        return(0);
    }

    // configure shared memory pointers
    unsigned long* numbers = create_shared_number_ptr(number_key);
    char* flags = create_shared_flag_ptr(flag_key);
    if(numbers == (unsigned long*)NULL){
        return(0);
    }

    if(flags == ((char*)NULL)){
        return(0);
    }
    
    
    shmctl(number_key, IPC_RMID, NULL);
    shmctl(flag_key, IPC_RMID, NULL);

    return(0);
}