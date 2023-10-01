#include "shm_func.h"


int main(int argc, char** argv[]){

    key_t key = get_file_key();
    if(key == 0){
        return(0);
    }

    char* sh_ptr = create_shared_ptr(key);
    if(sh_ptr == (char *)0){
        return(0);
    }

    strncpy(sh_ptr, "i am a test program", BLOCK_SIZE);
    
    shmctl(key, IPC_RMID, NULL);

    return(0);
}