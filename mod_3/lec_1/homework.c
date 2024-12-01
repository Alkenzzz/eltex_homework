#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#define FIFO_PATH "fifo_fn"
#define MESSAGE_HANDLER "SIGABRT handler called\n"
#define MESSAGE_SIGABRT "SIGABRT received!\n"
#define FIFO_MODE 0666
void sigabrt_handler(int sig) {
    int fd;
    char *messages[] = {MESSAGE_HANDLER, MESSAGE_SIGABRT};
    size_t num_messages = sizeof(messages) / sizeof(messages[0]);
    // Проверка ФИФО. Если его нет, то создаём.
    if (mkfifo(FIFO_PATH, FIFO_MODE) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo failed");
            exit(1);
        }
    }
    fd = open(FIFO_PATH, O_WRONLY);
    	if (fd == -1) {
        	perror("open failed");
        	exit(1);
    }
	for (size_t i = 0; i < num_messages; ++i) {
        	ssize_t bytes_written = write(fd, messages[i], strlen(messages[i]));
        	if (bytes_written == -1) {
            		perror("write failed");
            		close(fd);
            		exit(1);
        	} else if (bytes_written != strlen(messages[i])) {
            		fprintf(stderr, "Partial write: %zd bytes written\n", bytes_written);
            		close(fd);
            		exit(1);
        	}
    	}
    	if (close(fd) == -1) {
        perror("close failed");
    	}
	if (unlink(FIFO_PATH) == -1) {
        	perror("unlink failed");
    	}
    	exit(0);
}
int main() {
	struct sigaction sa = { .sa_handler = sigabrt_handler };
	if (sigaction(SIGABRT, &sa, NULL) == -1) {
		perror("sigaction failed");
		exit(1);
    	}
	printf("Waiting for SIGABRT. PID: %d\n", getpid());
    	pause();
    	return 0;
}


