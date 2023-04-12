#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <memory.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#endif


int main(){
    struct addrinfo hints;
    struct addrinfo *ai0;
    int i;
    // Set the hints:
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC; // Unspecified protocol (IPv4 or IPv6 okay)
    hints.ai_socktype = SOCK_STREAM; // Want a TCP socket

    // Perform a DNS lookup. The variable ai0 is set to point to the head
    // of a linked list of struct addrinfo values containing the possible
    // addresses of the server.
    if ((i = getaddrinfo("www.example.com", "80", &hints, &ai0)) != 0) {
        fprintf(stderr, "Error: unable to lookup IP address: %s", gai_strerror(i));
    }

    struct addrinfo *ai;
    int fd;
    // Iterate over the linked list of results of the DNS lookup, trying to
    // connect to each in turn, breaking out when successfully connected.
    for (ai = ai0; ai != NULL; ai = ai->ai_next) {
        fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (fd == -1) {
        // Unable to create socket, try next address in list
            continue;
        }
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == -1) {
            // couldn't connect to the address, try next in list
            close(fd);
            continue;
        }
        break; // successfully connected
    }
    if (ai == NULL) {
         // Couldn't connect to any of the addresses, handle failure...
    } else {
// Successfully connected: fd is a file descriptor of a socket
// connected to the server; use the connection

    }

    return 0;
}