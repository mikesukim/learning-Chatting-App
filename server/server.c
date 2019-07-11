// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>
#include  <sys/shm.h>
#include <sys/ipc.h> 

#define PORT 8080
#define MAX 80 

sem_t mutex;    
int connectionsDescription[100];

int shmid;
int *array;
int count = 100;
int i = 0;
int SizeMem;
key_t key = 12345;


void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n;

    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s", buff); 
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("One client Exit...\n"); 
            break; 
        }
        bzero(buff, MAX); 
        n = 0;  
    } 
} 

int main(int argc, char const *argv[])
{   

    pid_t childpid;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR /*| SO_REUSEPORT*/,
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
    //listen() defines queue which contains 
    //connections should be extablished at the accept(). 
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    while(1){
       // waits If there are no connection appended in listen queue.
       new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
       if(new_socket < 0){
           exit(1);
       }
       printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));


       if((childpid = fork()) == 0){
            close(server_fd);
        
            
            func(new_socket); 
        
       }


    }

    // After chatting close the socket 
    close(new_socket); 

    return 0;
}