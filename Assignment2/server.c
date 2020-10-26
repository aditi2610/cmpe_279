// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    struct passwd *pw;
    int num;

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
// check if the current user is root or not!
    if(geteuid())
        perror("Need Root Privilege\n");
    if((pw = getpwnam("nobody")) ==  NULL)
        perror("unknown user");
    num = fork();
    if(num == 0)
    {
        printf("Inside child\n");
        pw = getpwnam("nobody");
        if(setuid(pw->pw_uid) == -1){
            perror("couldnt drop privileges");
            return 0;
        }else {
            if(execl("util", "util", hello, &new_socket, NULL) < 0){
                perror("after exec");
                exit(EXIT_FAILURE);
            }else{
                perror("Couldnt exec");
                exit(EXIT_FAILURE);
            }
        }
    }else if(num >0){
        printf("Inside Parent\n");
        wait(2);
    } else {
        perror("Couldnt Fork");
        exit(2);
    }
   

    // if(pName != NULL){
    //      if(setgroups(1, &pw ->pw_gid) || setresgid(pw->pw_gid,pw -> pw_gid,pw -> pw_gid ) || setresuid(pw -> pw_uid, pw -> pw_uid, pw->pw_uid)){
    //              perror("Can't drop Privilige");
    //             }
    // }
    //     valread = read( new_socket , buffer, 1024);
    //     printf("%s\n",buffer );
    //     send(new_socket , hello , strlen(hello) , 0 );
    //     printf("Hello message sent\n");
  
   
    return 0;
}
