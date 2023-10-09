#include "shm_func.h"
#include <stdlib.h>

int main(int argc, char** argv[]){

    struct data_buffer{
        unsigned long num;
        char text[32];
        char* ptr;
    } buffer;

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
        printf("Please enter a number to be factorised: ");
        scanf("%s", buffer.text);
        buffer.num = strtoul(buffer.text, &buffer.ptr, 10);

        printf("%ld\n", buffer.num);

        numbers[0] = buffer.num;
        flags[0] = 'y';
        sleep(1);
        if(flags[0] == 'y'){
            flags[0] = 'n';
            printf("\x1b[1F"); // Move to beginning of previous line
            printf("\x1b[1F"); // Move to beginning of previous line
            printf("\x1b[2K"); // Clear entire line
            printf("\n");
            printf("\x1b[2K"); // Clear entire line
            printf("\x1b[1F"); // Move to beginning of previous line
            
            printf("Server is busy, try again..\n\n");
            
        }

    }
 
    // memory destructor
    shmctl(number_key, IPC_RMID, NULL);
    shmctl(flag_key, IPC_RMID, NULL);

    return(0);
}
