#include "shm_func.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


struct thread_data{
    int index;
    int bit_index;
    unsigned long number;
    unsigned long* shared_numbers;
    char* shared_flags;
    bool active;
    unsigned long bit_shifted_number[32];
};
/*
have two simultaneous functions using a shared array to pass factors back to client in real time
*/

unsigned long rotate_bits_right(unsigned long num, unsigned int shift){
    unsigned long result = (unsigned long)((num >> shift) | (num << 64 - shift));
    return(result);
}


void* factorise(void* arg){

    struct thread_data *thread;
    thread = (struct thread_data *) arg;

    int bit_index = thread->bit_index;
    unsigned long bit_shifted_number = thread->bit_shifted_number[bit_index];
    unsigned long square = sqrt(bit_shifted_number);

    
    for(unsigned long i = 2; i < square; i++){
        if(bit_shifted_number % i == 0){
            printf("%i, %lu, %.2f\n", bit_index, i, (float)i / square);
        }
    }
    
    
    return(NULL);
}

// uses trial division factor method to locate factors, and places them into ptr array 
void* factorise_all_rotated_bits(void* arg){

    struct thread_data *thread;
    thread = (struct thread_data *) arg;

    pthread_t pthreads[32];

    for(int i = 0; i < 32; i++){
        thread->bit_index = i;
        pthread_create(&pthreads[i], NULL, factorise, arg);
        
    }

    for(int i = 0; i < 32; i++){
        pthread_join(pthreads[i], NULL);
    }

    return(NULL);
}

int main(int argc, char** argv[]){

    pthread_t pthread[10];

    struct thread_data threads[10];

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
    

    // runtime loop
    while(1){
 
        if(flags[0] == 'y'){
            for(int i = 1; i < 10; i++){
                if(flags[0] == 'n'){continue;}

                if(threads[i].active == false){
                    threads[i].active = true;
                    threads[i].index = i;
                    threads[i].number = numbers[0];
                    threads[i].shared_numbers = numbers;
                    threads[i].shared_flags = flags;
                    for(int j = 0; j < 32; j++){
                        threads[i].bit_shifted_number[j] = rotate_bits_right(threads[i].number, j);
                    }
                    numbers[0] = i;
                    flags[0] = 'n';

                    pthread_create(&pthread[i], NULL, factorise_all_rotated_bits, (void*)&threads[i]);

                    sleep(1);

                }
            }
        }

    }

    

    // memory destructor
    shmctl(number_key, IPC_RMID, NULL);
    shmctl(flag_key, IPC_RMID, NULL);

    return(0);
}