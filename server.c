#include "shm_func.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* factorise(void* arg){

    unsigned long* input = (unsigned long*) arg;
    unsigned long* result = (unsigned long*) malloc(sizeof(unsigned long));

    *result = *input * (unsigned long)5;

    return(result);
}



int main(int argc, char** argv[]){

    struct server_data{
        pthread_t thread;
        unsigned long input;
        unsigned long* result;
    } data[10];

    data[0].input = 5;

    pthread_create(&data[0].thread, NULL, factorise, &data[0].input);
    pthread_join(data[0].thread, (void*)&data[0].result);

    printf("%ld\n", *data[0].result);
    


    

    key_t key = get_file_key();
    if(key == 0){
        return(0);
    }

    unsigned long* sh_ptr = create_shared_ptr(key);
    if(sh_ptr == (unsigned long*)NULL){
        return(0);
    }

    sh_ptr[0] = 7354;
    sh_ptr[1] = 11111195;
    
    shmctl(key, IPC_RMID, NULL);

    return(0);
}