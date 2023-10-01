#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#define BLOCK_SIZE 352

key_t get_file_key();
char* create_shared_ptr(key_t key);