#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define TIME_LIMIT 10

int main() {
    	int shmid;
    	double *shared_time;
    	char pathname[] = "player1.c";
    	key_t key = 123;

    	//Этап создания сегмента с разделяемой памятью

    	shmid = shmget(key, sizeof(double), IPC_CREAT | 0666);
    	if (shmid < 0) {
        	perror("shmget");
        	exit(1);
    	}

    	//Этап присоединения к сегменту разделяемой памяти

    	shared_time = (double *)shmat(shmid, NULL, 0);
    	if (shared_time == (double *) -1) {
        	perror("shmat");
       		exit(1);
    	}

	//Этап основной. Определение хода + определение времени с использованием библиотеки time.h

    	*shared_time = 0;
    	while (1) {
        	printf("Ход белых. Нажмите Enter, чтобы завершить ход...\n");
        	getchar();

        	double start_time = (double)clock() / _SC_CLK_TCK;
        	double elapsed_time = (double)clock() / _SC_CLK_TCK - start_time;
        	*shared_time += elapsed_time;

        	if (*shared_time > TIME_LIMIT) {
            		printf("Техническое поражение белых\n");
            		break;
        	}
        	printf("Время белых: %.2f секунд\n", *shared_time);
    	}
    	shmdt(shared_time);
    	return 0;
}
