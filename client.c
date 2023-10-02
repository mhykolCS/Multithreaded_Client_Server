#include "shm_func.h"
#include <stdlib.h>

int main(int argc, char** argv[]){

    struct client_data{
        unsigned long number;
        char text[32];
        char* str_ptr;
    } user_input;

    key_t key = get_file_key();
    if(key == 0){
        return(0);
    }

    unsigned long* sh_ptr = create_shared_ptr(key);
    if(sh_ptr == (unsigned long*)NULL){
        return(0);
    }

    
    while(1){
        printf("Please enter a number to be factored: ");
        scanf("%32s", user_input.text);

        user_input.number = strtoul(user_input.text, &user_input.str_ptr, 10);

        if(user_input.number == 0){
            if(user_input.text[0] == 'q' || user_input.text[0] == 'Q'){
                break;
            }
        }

        printf("%ld\n", user_input.number);
        
        

    }


    shmctl(key, IPC_RMID, NULL);

    return(0);
}