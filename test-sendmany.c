#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include "sp.h"

int main (int argc, char *argv[])
{
    int sock, rc;

    if (argc < 3) {
        fprintf (stderr, "usage: test-sendmsg HOST PORT\n");
        exit (1);
    }

    sock = socket (PF_SP, SOCK_PUB, 0);
    if (sock < 0)
        printf ("socket() failed: %s\n", strerror (errno));

    struct addrinfo req, *res;
    memset (&req, 0, sizeof (req));
    req.ai_family = AF_INET;
    req.ai_socktype = SOCK_STREAM;
    req.ai_flags = AI_NUMERICSERV;
    rc = getaddrinfo (argv[1], argv[2], &req, &res);
    if (rc) {
        printf ("getaddinfo failed\n");
        exit (1);
    }
    rc = connect (sock, res->ai_addr, res->ai_addrlen);
    if (rc < 0)
        printf ("connect() failed: %s\n", strerror (errno));
    freeaddrinfo (res);

    char message[251];
    memset(message, 'A', sizeof message);
    while (1) {
        ssize_t nbytes;
        nbytes = send (sock, message, sizeof message, 0);
        if (nbytes < 0) {
            printf ("send() failed: %s\n", strerror (errno));
        }
        else if (nbytes != 251) {
            printf ("send() sent only %d bytes\n", (int)nbytes);
            exit(0);
        }
    }

    rc = close (sock);
    if (rc < 0)
        printf ("close() failed: %s\n", strerror (errno));
    exit (0);
}

