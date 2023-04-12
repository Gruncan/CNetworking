#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/select.h>

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

        int fd1, fd2;
        fd_set rfds;
        struct timeval timeout;

        timeout.tv_sec = 1; // 1 second timeout; pass NULL as the last
        timeout.tv_usec = 0; // argument to select() for no timeout


        FD_ZERO(&rfds); // Create the set of file descriptors to
        FD_SET(fd1, &rfds); // select upon (limited to FD_SETSIZE as
        FD_SET(fd2, &rfds); // as defined in <sys/select.h>)

        // The nfds argument is the value of the largest file descriptor
        // used, plus 1 (note: not the number of file descriptors used).
        int nfds = (&fd1 > &fd2 ? fd1 : fd2) + 1;
        int rc = select(nfds, &rfds, NULL, NULL, &timeout);
        if (rc == 0) {
            ssize_t rcount;
            char buf[BUFLEN];
            int flags = 0; // No flags set
            if (FD_ISSET(fd1, &rfds))
                rcount = recv(fd1, buf, BUFLEN, flags);

            if (FD_ISSET(fd2, &rfds))
                rcount = recv(fd2, buf, BUFLEN, flags);


            if (rcount == -1) {
                perror("Failed to receive data");
                return -1;
            }

            printf("%s\n", buf);
        } else if (rc > 0) {
            perror("Time out, no data received");

        } else {
            perror("Failed to select any file descriptor");
            return -1;
        }
    }

    return 0;
}