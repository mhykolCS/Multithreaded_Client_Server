program: client.c server.c shm_func.h shm_func.c
	gcc client.c shm_func.c -o client
	gcc -lpthread server.c shm_func.c -o server -lm

clean:
	rm client
	rm server