// NUMBER OF EVENTS (MAX_EVENTS) DOESNT SEEMS TO CHANGE SOMETHING DONT UNDERSTAND
// Exit, perror, printf, doesnt respect C++98
 
#include <stdio.h> // printf, perror
#include <stdlib.h> // exit, malloc
#include <string.h> // memset
#include <unistd.h> // close, write
#include <fcntl.h> // fcntl
#include <sys/epoll.h> // epoll, recv
#include <netinet/in.h> // define struct and macros for network operations IP socket.
#include <arpa/inet.h> // inet_ntoa

#define MAX_EVENTS 10 // Maximum number of events that epoll can handle
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
    // Set SO_REUSEADDR to reuse the port immediately after the program exits
    int optval = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        close(sfd);
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
    return 0;
}

// Function to get the buffer sizes of a socket
int getSocketBufferSize(int sfd, int *recvBufSize, int *sendBufSize)
{
    socklen_t optlen;

    // Get receive buffer size
    optlen = sizeof(*recvBufSize);
    if (getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, recvBufSize, &optlen) == -1)
    {
        perror("getsockopt SO_RCVBUF");
        return -1;
    }

    // Get send buffer size
    optlen = sizeof(*sendBufSize);
    if (getsockopt(sfd, SOL_SOCKET, SO_SNDBUF, sendBufSize, &optlen) == -1)
    {
        perror("getsockopt SO_SNDBUF");
        return -1;
    }
    return 0; // Success
}

// Function to set the buffer sizes of a socket
int setSocketBufferSize(int sfd, int recvBufSize, int sendBufSize)
{   
    // Set receive buffer size
    if (setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &recvBufSize, sizeof(recvBufSize)) == -1)
    {
        perror("setsockopt SO_RCVBUF");
        return -1;
    }

    // Set send buffer size
    if (setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &sendBufSize, sizeof(sendBufSize)) == -1)
    {
        perror("setsockopt SO_SNDBUF");
        return -1;
    }
    //Check if it worked.
    printf("Client Socket - Receive Buffer Size: %d, Send Buffer Size: %d\n", recvBufSize, sendBufSize);
    return 0;
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
        perror("makeSocketNonBlocking");
        return;
    }
    if(setSocketBufferSize(client_sock, 65536, 65536) == -1) //Set buffer sizes for the client socket to 64ko because its well balanced between perf and memory and usualy used. The sizes of the buffer should be a multiple of MTU (maximum transmission unit) to avoid packet fragmentation ask google for more informations.
    {
        close(client_sock);
        perror("setSocketBufferSize");
        return;
    }
    addToEpoll(epoll_fd, client_sock, EPOLLIN); // Add the client socket to the epoll instance
}

// Function to handle data received from a client
void handleClientData(int epoll_fd, int client_sock)
{
    char buffer[1024];
    ssize_t count = recv(client_sock, buffer, sizeof(buffer), 0); // Read data from the client
    if (count == -1)
    {
        perror("recv");
        close(client_sock);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
    }
    else if (count == 0) // Client closed the connection
    {
        printf("Client disconnected\n");
        close(client_sock);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
    }
    else
    {
        buffer[count] = '\0'; // Null-terminate the request
        printf("Request: %s\n", buffer);

        // Determine the file to serve based on the request
        const char *file_path = NULL;
        const char *content_type = NULL;
        if (strstr(buffer, "GET /style.css") != NULL)
        {
            file_path = "./errors/40x/404.css";
            content_type = "text/css";
        }
        else if (strstr(buffer, "GET /") != NULL || strstr(buffer, "GET /index.html") != NULL)
        {
            file_path = "./errors/50x/504.html";
            content_type = "text/html";
        }
        else
        {
            file_path = "./errors/40x/404.html";
            content_type = "text/html";
        }

        // Read the file
        FILE *file = fopen(file_path, "r");
        if (!file)
        {
            perror("fopen");
            close(client_sock);
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
            return;
        }

        // Get the file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Allocate buffer to read the file
        char *file_buffer = (char *)malloc(file_size + 1);
        if (!file_buffer)
        {
            perror("malloc");
            fclose(file);
            close(client_sock);
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
            return;
        }
        fread(file_buffer, 1, file_size, file);
        file_buffer[file_size] = '\0'; // Null-terminate the buffer

        // Send HTTP headers
        const char *response_headers_template = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n";
        char response_headers[256];
        snprintf(response_headers, sizeof(response_headers), response_headers_template, content_type, file_size);

        write(client_sock, response_headers, strlen(response_headers));

        // Send the file content to the client
        write(client_sock, file_buffer, file_size);
        free(file_buffer);
        fclose(file);

        // Close the connection after sending the response
        shutdown(client_sock, SHUT_WR); // Close the write end of the socket
        close(client_sock);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_sock, NULL); // Remove the client from the epoll instance
    }
//        write(client_sock, buffer, count); // Echo the data back to the client
//        shutdown(client_sock, SHUT_WR); // Close the write end of the socket 
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
            printf("n = %d\n", n); // Print the number of events received (DOESTNT WORK)
        }
    }
    close(listen_sock); // Close the listening socket
    close(epoll_fd); // Close the epoll instance
    return 0;
}