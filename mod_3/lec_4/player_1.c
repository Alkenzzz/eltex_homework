#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <time.h>
//Задаю макрос:
#define SHM_NAME "/chess_shm"
#define SEM_WHITE "/sem_white"
#define SEM_BLACK "/sem_black"
#define SHM_SIZE 100
//P.S в комментариях 1й - это белые, 2й - это чёрные:
int main() {
	int shm_fd;
	char *shared_memory;
	sem_t *sem_white, *sem_black;
	char move[SHM_SIZE];
	srand(time(NULL));
//Создание или открытие разделяемой памяти:
	shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    	if (shm_fd == -1) {
        	perror("shm_open");
        	exit(1);
    	}
//Сначала задаём память. Затем отобразим разделяемую память в адрес.пространство процесса:
	ftruncate(shm_fd, SHM_SIZE);
    	shared_memory = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    	if (shared_memory == MAP_FAILED) {
        	perror("mmap");
        	exit(1);
    	}
// Создание семафоров:
    	sem_white = sem_open(SEM_WHITE, O_CREAT | O_EXCL, 0644, 1);  // Первый ход 1го игрока
    	if(sem_white == SEM_FAILED) {
        	if(errno == EEXIST){
            		sem_white = sem_open(SEM_WHITE, 0);
            		if (sem_white == SEM_FAILED) {
                  		perror("sem_open (white, открытие)");
                  		munmap(shared_memory, SHM_SIZE);
                  		close(shm_fd);
                    		exit(1);
                		}
        		} else {
            	perror("sem_open (white, создание)");
              	munmap(shared_memory, SHM_SIZE);
              	close(shm_fd);
               	exit(1);
        	}
    		}  else {
            		printf("Семафор белых создан успешно.\n");
      		}
   	sem_black = sem_open(SEM_BLACK, O_CREAT | O_EXCL, 0644, 0); // 2й игрок ждёт 1го хода
        if(sem_black == SEM_FAILED) {
             	if(errno == EEXIST){
                 	sem_black = sem_open(SEM_BLACK, 0);
                  	if (sem_black == SEM_FAILED) {
                      		perror("sem_open (black, открытие)");
                      	munmap(shared_memory, SHM_SIZE);
                      	close(shm_fd);
                       	exit(1);
                  }
             		} else {
                  		perror("sem_open (black, создание)");
                  		munmap(shared_memory, SHM_SIZE);
                  		close(shm_fd);
                  		exit(1);
             	}
        	} else {
              		printf("Семафор черных создан успешно.\n");
        	}
// Шахматные ходы для 1го:
    	char* white_moves[] = {"d4", "с3", "с4", "d4", "с6", NULL};
	int move_index = 0;
    	while(white_moves[move_index] != NULL){
        	sem_wait(sem_white);   //Ожидаем семафор 1го
        	strcpy(shared_memory, white_moves[move_index]);
        	printf("Белые: %s\n", shared_memory);
        	move_index++;
        sem_post(sem_black);   // Разрешаем ход 2му
        sleep(1); //Задержка
    }
    	munmap(shared_memory, SHM_SIZE);
    	close(shm_fd);
    	sem_close(sem_white);
    	sem_close(sem_black);
	if(sem_unlink(SEM_WHITE) == -1) {
        	perror("sem_unlink (white)"); // Ошибка, но не завершаем
     	}
     	if(sem_unlink(SEM_BLACK) == -1) {
        perror("sem_unlink (black)"); // Ошибка, но не завершаем
     	}
    	if(shm_unlink(SHM_NAME) == -1) {
        	perror("shm_unlink");
     	}
// Определение победителя(За счёт рандома):
	int winner = rand() % 2; // 0 для белых, 1 для черных
    	if (winner == 0) {
       		printf("Победа \"Белых\"\n");
    	} else {
       		printf("Победа \"Чёрных\"\n");
    	}

	return 0;
	}
