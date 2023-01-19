#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>


#define BUFLEN 1500

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("File Descriptor Error");
        return -1;
    }

    int port = 8888;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("Bind Error");
        return -1;
    }

    int backlog = 10;

    if(listen(fd, backlog) == -1){
        return -1;
    }

    printf("Listening on port %d\n", port);

    while (1){
        struct sockaddr_in cliaddr;
        socklen_t cliaddr_len = sizeof(cliaddr);
        int connfd = accept(fd, (struct sockaddr*) &cliaddr, &cliaddr_len);
        if(connfd == -1){
            perror("Failed to accept connection");
            return -1;
        }
        ssize_t rcount;
        char buf[BUFLEN];
        int flags = 0; // No flags set
        rcount = recv(connfd, buf, BUFLEN, flags);
        if (rcount == -1) {
            perror("Failed to receive data");
            return -1;
        }

        printf("%s\n", buf);


    }

    return 0;
}