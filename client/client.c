#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <unistd.h>

int main(){

    struct addrinfo hints;
    struct addrinfo *ai0;
    int i;

    // Set the hints:
    memset(&hints, 0, sizeof(hints));
    // Unspecified protocol (IPv4 or IPv6 okay)
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // Want a TCP socket


    // Perform a DNS lookup. The variable ai0 is set to point to the head
    // of a linked list of struct addrinfo values containing the possible
    // addresses of the server.
    if ((i = getaddrinfo("127.0.0.1", "8888", &hints, &ai0)) != 0) {
        printf("Error: unable to lookup IP address: %s", gai_strerror(i));
    }

    struct addrinfo *ai;
    int fd;


    // Iterate over the linked list of results of the DNS lookup, trying to
    // connect to each in turn, breaking out when successfully connected.
    for (ai = ai0; ai != NULL; ai = ai->ai_next) {
        fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        // Unable to create socket, try next address in list
        if (fd == -1) continue;

        // couldn't connect to the address, try next in list
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == -1) {
            close(fd);
            continue;
        }
        // successfully connected
        break;
    }

    if (ai == NULL) {
        printf("Couldn't find any address to connect to\n");
        return -1;
    }

    char *data = "Hello World!";
    size_t data_len = strlen(data);
    int flags = MSG_NOSIGNAL;


    if (send(fd, data, data_len, flags) == -1) {
        perror("Failed to send");
        return -1;
    }




    return 0;
}