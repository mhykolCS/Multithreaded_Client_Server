#include "shm_func.h"
#include <pthread.h>
#include <stdbool.h>


struct thread_data{
    int index;
    int count;
    unsigned long* number;
    char* flag;
    bool active;
};
/*
have two simultaneous functions using a shared array to pass factors back to client in real time
*/


// returns the amount of binary digits in the unsigned long
int binary_count(unsigned long num){
    int temp = num;
    int count = 0;
    for (count; temp > 0; temp >>= 1){
        count++;
    }

    return(count);

}

int right_rotation(int num, unsigned int shift){
    return (num >> shift) | (num << 32 - shift);
}


// uses trial division factor method to locate factors, and places them into ptr array 
void* factorise(void* arg){

    struct thread_data *thread;
    thread = (struct thread_data *) arg;

    printf("Index: %d\n", thread->index);
    printf("Count: %d\n", thread->count);
    printf("Number: %ld\n", *thread->number);
    printf("Flag: %s\n\n", thread->flag);  

    *thread->flag = 'n';
    thread->active = false;

    return(NULL);
}

int main(int argc, char** argv[]){

    pthread_t thread1;

    struct thread_data threads;

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
        if(flags[0] == 'y' && !threads.active){
            threads.index = 0;
            threads.count = binary_count(numbers[0]);
            threads.number = &numbers[0];
            threads.flag = &flags[0];
            if(!threads.active){
                threads.active = true;
                pthread_create(&thread1, NULL, factorise, (void *)&threads);
                sleep(1);
            }    
        }
    }

    

    // memory destructor
    shmctl(number_key, IPC_RMID, NULL);
    shmctl(flag_key, IPC_RMID, NULL);

    return(0);
}