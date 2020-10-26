#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>

int
main(int argc, char const *args[]){
  int valread, socket;
  const char *msg;
  char buffer[1024] = {0};

    printf("uid:%i\n",getuid());
    if (argc == 3) {
        msg = argv[1];
        socket = *argv[2];
    }
    else {
        perror("Args != 3");
        exit(EXIT_FAILURE);
    }
    valread = read( socket , buffer, 1024);
    printf("%s\n",buffer );
    send(socket , msg , strlen(msg) , 0);
    printf("Hello message sent\n");
    return 0;
}