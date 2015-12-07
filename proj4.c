#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main()
{
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    
    if (sock < 0)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server;
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    
    unsigned short listener_port = 8765;
    
    server.sin_port = htons(listener_port);
    int len = sizeof(server);
    
    if (bind(sock, (struct sockaddr*)&server, len) < 0)
    {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }
    
    listen(sock, 5);
    printf("PARENT: Listener bound to port %d\n", listener_port);
    
    struct sockaddr_in client;
    int fromlen = sizeof(client);
    
    int pid;
    char buffer[1024];
    
    while(1)
    {
        
        printf("PARENT: Blocked on accept()\n");
        
        int newsock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&fromlen);
        
        printf("PARENT: Accepted client connection\n");
        
        printf("THREAD: Blocked on read()\n");
        
        int n = read(newsock, buffer, 1024);
        
        if (n < 0)
        {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        
        printf("MESSAGE: %s", buffer);
        
    }
    
}