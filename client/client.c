// Client side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX 80 
int sock = 0;

void *Reading()
{

    char buff[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        n = 0; 
        if( read(sock, buff, sizeof(buff)) < 0){
            // printf("Error receiving data");
        }
        else{
            printf("MESSAGE RECEIVED : %s",buff);
        }
        bzero(buff, sizeof(buff));  
    } 

   pthread_exit(NULL);
}

void *Writing()
{

    char buff[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        // printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
        write(sock, buff, sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        }
        bzero(buff, sizeof(buff));  
    } 

   pthread_exit(NULL);
}

void func(int sockfd) 
{ 

    pthread_t thread1;

    pthread_create(&thread1, NULL, Reading, NULL);

    char buff[MAX]; 
    int n; 

    for (;;) { 
        bzero(buff, sizeof(buff)); 
        // printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
        write(sock, buff, sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            pthread_kill(thread1,0);
            printf("comes here??");
            break; 
           
        }
        bzero(buff, sizeof(buff));  
    } 


    // for (;;) { 
    //     bzero(buff, sizeof(buff)); 
    //     printf("Enter the string : "); 
    //     n = 0; 
    //     while ((buff[n++] = getchar()) != '\n') 
    //         ; 
    //     write(sock, buff, sizeof(buff));
    //     if ((strncmp(buff, "exit", 4)) == 0) { 
    //         printf("Client Exit...\n"); 
    //         break; 
    //     }
    
    //     // printf("From Server : %s", buff); 
    //     if( read(sock, buff, sizeof(buff)) < 0){
    //         printf("Error receiving data");
    //     }
    //     else{
    //         printf("%s\n",buff);
    //     }

    //     bzero(buff, sizeof(buff));  
    // }

    // pthread_join(thread1,NULL); 
}


int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // send(sock , hello , strlen(hello) , 0 );
    // printf("Hello message sent\n");
    // valread = read( sock , buffer, 1024);
    // printf("%s\n",buffer );

     // function for chat 
    func(sock); 
  
    // close the socket 
    close(sock); 

    
    return 0;
}