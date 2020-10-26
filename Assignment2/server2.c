#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    struct passwd* pwd;

    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int proc_number = fork();
    if(proc_number == 0) {
	printf("in child process: %i\n",getpid());
	pwd = getpwnam("nobody");
	if(setuid(pwd->pw_uid) == -1){
            perror("privilege drop failed");
	    exit(EXIT_FAILURE);
	} else {
            if(execl("child", "child", hello, &new_socket, NULL) < 0){
                perror("child process ended with exec");
                exit(EXIT_FAILURE);
            } else {
                perror("Child exited with error");
                exit(EXIT_FAILURE);
            }
	}
    } else if(proc_number > 0) {
	printf("in parent process: %i\n",getpid());
	printf("parent uid: %i\n",getuid());
	int wait_number = wait(NULL);
	if(wait_number < 0){
	    perror("wait failed");
	    exit(EXIT_FAILURE);	
	}
	printf("parent wait completed");
    } else {
    	perror("fork error");
	exit(EXIT_FAILURE);
    }
    return 0;
}