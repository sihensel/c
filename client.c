/*
    Simple clients that connects to a server via sockets and sends and
    receives messages
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>


int main(void)
{
    int sock;                       // socket
    struct sockaddr_in server;      // server address struct
    char *msg, reply[1000];         // message to server and reply

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Error creating socket: %s\n", strerror(errno));
        return 1;
    }

    // create the server address struct
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);

    // we need to typecast `sockaddr_in` to `sockaddr`
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Error connecting to server: %s\n", strerror(errno));
        return 1;
    }
    puts("Connected to server");

    // send a message to the server
    msg = "GET /index.html HTTP/1.1\nHost: myhost.com\nAccept: */*\n\n";
    if (send(sock, msg, strlen(msg), 0) < 0) {
        printf("Error sending message: %s\n", strerror(errno));
        return 1;
    }

    // get the reply
    if (recv(sock, reply, sizeof(reply), 0) < 0) {
        printf("Error receiving message: %s\n", strerror(errno));
        return 1;
    }
    printf("Reply from server:\n%s\n", reply);

    close(sock);

    return 0;
}
