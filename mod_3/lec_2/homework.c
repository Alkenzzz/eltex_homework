#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
   	pid_t my_pid = getpid();
    	printf("My PID: %d\n", my_pid);

    	sleep(1);

    	pid_t child_pid = fork();

	//Дочерний процесс:

    	if (child_pid == 0) {
		//Для вывода сообщения создал буфер, с указанием размера
        	char message[1024] = "";

        	for (int i = 1; i < argc; i++) {
            		strcat(message, argv[i]);
            		if (i < argc - 1) {
                		strcat(message, " ");
            		}
        	}

		printf("Дочерний процесс: %d, Сообщение: %s\n", getpid(), message);

    	} else if (child_pid < 0) {
        	perror("fork failed");
        	return 1;
	//Родитель:
    	} else {
        	int status;
        	wait(&status);
    }
    return 0;
}


