#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>



int main(){


    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        fprintf(stderr, "Failed to establish connection");
        return -1;
    }


    struct sockaddr_in addr;
    addr.sin_family = AF_INET; // IPv4
    addr.sin_addr.s_addr = INADDR_ANY; // Any available network interface
    addr.sin_port = htons(80); // Port 80
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Failed to bind address");
        return -1;
    }

    int backlog = 10;
    if (listen(fd, backlog) == -1) {
        fprintf(stderr, "Failed to start listening");
        return -1;
    }

    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t cliaddr_len = sizeof(cliaddr);
        int connfd = accept(fd, (struct sockaddr *) &cliaddr, &cliaddr_len);
        if (connfd == -1) {
            fprintf(stderr, "Failed to accepted client!");
        }

        ...
    }

    return 0;
}