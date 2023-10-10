#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

key_t get_file_key(char* key_string);
unsigned long* create_shared_number_ptr(key_t key);
char* create_shared_flag_ptr(key_t key);