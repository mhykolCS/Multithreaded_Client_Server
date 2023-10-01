program: client.c server.c shm_func.h shm_func.c
	gcc client.c shm_func.c -o client
	gcc server.c shm_func.c -o server

clean:
	rm client
	rm server