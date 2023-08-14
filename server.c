/*
    Simple server that binds to a socket on port 1234 and accepts incoming
    connections and messages
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>


int main(void)
{
    int sock, new_sock;                 // sockets are file descriptors
    struct sockaddr_in server, client;  // server and client addresses
    socklen_t client_len;               // length of client address
    char *client_ip;                    // client IP address
    int client_port;                    // port the client is connecting from
    char *msg, client_msg[1000];        // message to and from the client

    // AF_INET = IPv4, SOCK_STREAM = TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Error creating socket: %s\n", strerror(errno));
        return 1;
    }

    // create the server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1234);

    // we need to typecast `sockaddr_in` to `sockaddr`
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Error binding socket: %s\n", strerror(errno));
        return 1;
    }

    // accept max. 3 connections
    listen(sock, 3);
    puts("Waiting for incoming connections...");

    client_len = sizeof(client);
    new_sock = accept(sock, (struct sockaddr *) &client, &client_len);
    if (new_sock < 0) {
        printf("Error accepting connection: %s\n", strerror(errno));
        return 1;
    }

    client_ip = inet_ntoa(client.sin_addr);
    client_port = ntohs(client.sin_port);
    printf("Client %s connected from port %d\n", client_ip, client_port);

    // receive message from client
    recv(new_sock, client_msg, sizeof(client_msg), 0);
    printf("Message from client:\n%s\n", client_msg);

    // send response to client
    msg = "HTTP/1.1 200 OK\n";
    send(new_sock, msg, strlen(msg), 0);

    close(new_sock);
    close(sock);

    return 0;
}
