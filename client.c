#include "shm_func.h"
#include <stdlib.h>

int main(int argc, char** argv[]){

    key_t key = get_file_key();
    if(key == 0){
        return(0);
    }

    unsigned long* sh_ptr = create_shared_ptr(key);
    if(sh_ptr == (unsigned long*)NULL){
        return(0);
    }

    

    shmctl(key, IPC_RMID, NULL);

    return(0);
}
