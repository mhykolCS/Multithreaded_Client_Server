#include "shm_func.h"
#include <stdlib.h>

struct shared_memory_data{
    unsigned long* numbers;
    char* flags;
};

void* process_data(void* arg){

    struct shared_memory_data *shared_memory;
    shared_memory = (struct shared_memory_data *) arg;

    while(1){
        for(int i = 1; i <= 10; i++){
            if(shared_memory->flags[i] == 'f'){
                shared_memory->flags[1] = 'e';
                printf("ID: %i, NUMBER: %lu\n", i, shared_memory->numbers[i]);
            }
        }
    }


    return(NULL);
}


int main(int argc, char** argv[]){

    struct data_buffer{
        unsigned long num;
        char text[32];
        char* ptr;
    } buffer;

    struct shared_memory_data shared_memory;

    pthread_t data_thread;

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

    shared_memory.numbers = numbers;
    shared_memory.flags = flags;
    if(numbers == (unsigned long*)NULL){
        return(0);
    }

    if(flags == ((char*)NULL)){
        return(0);
    }

    pthread_create(&data_thread, NULL, process_data, (void*)&shared_memory);

    // runtime loop
    do{
        
        printf("Please enter a number to be factorised: ");
        scanf("%s", buffer.text);
        buffer.num = strtoul(buffer.text, &buffer.ptr, 10);

        printf("%ld\n", buffer.num);

        numbers[0] = buffer.num;
        flags[0] = 'f';
        sleep(1);
        if(flags[0] == 'f'){
            flags[0] = 'e';
            
            printf("Server is busy, try again..\n\n");
            
        }

    }while(buffer.text[0] != 'q');
 
    // memory destructor
    shmctl(number_key, IPC_RMID, NULL);
    shmctl(flag_key, IPC_RMID, NULL);

    return(0);
}
