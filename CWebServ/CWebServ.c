#include <stdio.h> // printf, perror
#include <stdlib.h> // exit, malloc
#include <string.h> // memset
#include <unistd.h> // close, read, write
#include <fcntl.h> // fcntl
#include <sys/epoll.h> // epoll
#include <netinet/in.h> // define struct and macros for network operations IP socket.
#include <arpa/inet.h> // inet_ntoa

#define MAX_EVENTS 1 // Maximum number of events that epoll can handle
#define PORT 8080 // Port number on which the server will listen

// Function to create and bind a socket to the specified port
int createAndBindSocket(int port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if(sfd == -1)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr)); // Zero out the address structure
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // Set the port to listen on
    addr.sin_addr.s_addr = INADDR_ANY; // Listen on any available network interface
    if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) // Bind the socket
    {
        perror("bind");
        close(sfd);
        return -1;
    }
    if(listen(sfd, SOMAXCONN) == -1) // Listen for incoming connections
    {
        perror("listen");
        close(sfd);
        return -1;
    }
    printf("IP = %s\n", inet_ntoa(addr.sin_addr)); // Print the IP address being used
    return sfd; // Return the socket file descriptor
}

// Function to make a socket non-blocking
int makeSocketNonBlocking(int sfd)
{
    int flags = fcntl(sfd, F_GETFL, 0); // Get current flags of the socket
    if (flags == -1)
    {
        perror("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK; // Set the socket to non-blocking mode
    if (fcntl(sfd, F_SETFL, flags) == -1) // Update the flags
    {
        perror("fcntl");
        return -1;
    }
    return 0; // Success
}

// Function to add a file descriptor to the epoll instance
void addToEpoll(int epoll_fd, int fd, uint32_t events)
{
    struct epoll_event event;
    event.events = events; // Specify the events to monitor (EPOLLIN)
    event.data.fd = fd; // Associate the file descriptor with the event
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) // Add the fd to the epoll instance
    {
        perror("epoll_ctl");
        close(fd);
    }
}

// Function to handle a new incoming connection
void handleNewConnection(int epoll_fd, int listen_sock)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_len); // Accept the connection
    if(client_sock == -1)
    {
        perror("accept");
        return;
    }
    if(makeSocketNonBlocking(client_sock) == -1) // Make the client socket non-blocking
    {
        close(client_sock);
        return;
    }
    addToEpoll(epoll_fd, client_sock, EPOLLIN); // Add the client socket to the epoll instance
}

// Function to handle data received from a client
void handleClientData(int epoll_fd, int client_sock)
{
    char buffer[512];
    int count = read(client_sock, buffer, sizeof(buffer)); // Read data from the client
    if(count == -1)
    {
        perror("read");
        close(client_sock);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
    }
    else if (count == 0) // Client closed the connection
    {
        close(client_sock);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
    }
    else
    {
        write(client_sock, buffer, count); // Echo the data back to the client
        shutdown(client_sock, SHUT_WR); // Close the write end of the socket
    }
}

int main()
{
    int listen_sock = createAndBindSocket(PORT); // Create and bind the listening socket
    if (listen_sock == -1)
    {
        exit(EXIT_FAILURE);
    }
    if(makeSocketNonBlocking(listen_sock) == -1) // Make the listening socket non-blocking
    {
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0); // Create an epoll instance
    if(epoll_fd == -1)
    {
        perror("epoll_create1");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    addToEpoll(epoll_fd, listen_sock, EPOLLIN); // Add the listening socket to the epoll instance

    struct epoll_event events[MAX_EVENTS];
    printf("----STARTING TO LISTENING----\n");
    while(1)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // Wait for events
        if(n == -1)
        {
            perror("epoll_wait");
            close(listen_sock);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }
        printf("n = %d", n); // Print the number of events received (DOESTNT WORK)
        for (int i = 0; i < n; i++) // Process each event
        {
            if(events[i].data.fd == listen_sock) // Check if the event is on the listening socket
            {
                handleNewConnection(epoll_fd, listen_sock); // Handle new connections
            }
            else
            {
                handleClientData(epoll_fd, events[i].data.fd); // Handle data from a client
            }
        }
    }
    close(listen_sock); // Close the listening socket
    close(epoll_fd); // Close the epoll instance
    return 0;
}