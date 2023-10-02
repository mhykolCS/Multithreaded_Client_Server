#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

key_t get_file_key();
unsigned long* create_shared_ptr(key_t key);