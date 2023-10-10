#include "shm_func.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct thread_data{
    int index;
    int bit_index;
    unsigned long number;
    unsigned long* shared_numbers;
    char* shared_flags;
    bool active;
    unsigned long bit_shifted_number[32];
};

struct multithread_data{
    int index;
    int bit_index;
    unsigned long bit_shifted_number;
    unsigned long* shared_numbers;
    char* shared_flags;
};


unsigned long rotate_bits_right(unsigned long num, unsigned int shift){
    unsigned long result = (unsigned long)((num >> shift) | (num << 64 - shift));
    return(result);
}


void* factorise(void* arg){
    struct multithread_data *thread;
    thread = (struct multithread_data *) arg;

    int bit_index = thread->bit_index;
    unsigned long bit_shifted_number = thread->bit_shifted_number;
    unsigned long square = sqrt(bit_shifted_number);

    
    for(unsigned long i = 2; i < square; i++){
        if(bit_index > 32 || bit_index < -1){
            i--;
            continue;
        }
        if(thread->index > 11 || thread->index < 0){
            i--;
            continue;
        }
        if(bit_shifted_number % i == 0){
            printf("Index: %i, BitIndex: %i, Factor: %lu, Percentage: %.2f%%\n", thread->index, thread->bit_index, i, (float)i / square);
            pthread_mutex_lock(&lock);
            while(thread->shared_flags[thread->index] == 'f'){
                continue;
            }

            thread->shared_numbers[thread->index] = i;
            thread->shared_flags[thread->index] = 'f';
            pthread_mutex_unlock(&lock);
        }
    }
    
    
    return(NULL);
}

// uses trial division factor method to locate factors, and places them into ptr array 
void* factorise_all_rotated_bits(void* arg){

    struct thread_data *thread;
    thread = (struct thread_data *) arg;
    int thread_index = thread->index;

    pthread_t pthreads[32];
    struct multithread_data multithreads[32];

    for(int i = 0; i < 32; i++){
        multithreads[i].index = thread_index;
        multithreads[i].bit_index = i;
        multithreads[i].bit_shifted_number = thread->bit_shifted_number[i];
        multithreads[i].shared_flags = thread->shared_flags;
        multithreads[i].shared_numbers = thread->shared_numbers;
    }

    for(int i = 0; i < 32; i++){
        pthread_create(&pthreads[i], NULL, factorise, (void*)&multithreads[i]);
    }

    return(NULL);
}

int main(int argc, char** argv[]){

    pthread_t pthread[10];

    struct thread_data threads[11];

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
 
        if(flags[0] == 'f'){
            for(int i = 1; i <= 10; i++){
                if(flags[0] == 'e'){continue;}

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
                    flags[0] = 'e';

                    pthread_create(&pthread[i], NULL, factorise_all_rotated_bits, (void*)&threads[i]);

                }
            }
        }

    }

    

    // memory destructor
    shmctl(number_key, IPC_RMID, NULL);
    shmctl(flag_key, IPC_RMID, NULL);

    return(0);
}