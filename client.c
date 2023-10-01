#include "shm_func.h"


int main(int argc, char** argv[]){

    key_t key = get_file_key();
    if(key == 0){
        return(0);
    }

    int* sh_ptr = create_shared_ptr(key);
    if(sh_ptr == (int *)NULL){
        return(0);
    }

    printf("%d\n", sh_ptr[0]);
    printf("%d\n", sh_ptr[1]);

    shmctl(key, IPC_RMID, NULL);

    return(0);
}