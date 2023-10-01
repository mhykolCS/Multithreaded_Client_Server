#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#define BLOCK_SIZE 44 

key_t get_file_key();
int* create_shared_ptr(key_t key);