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

    char buffer[1234];
    ssize_t nbytes;
    nbytes = recv (sock, buffer, sizeof buffer, 0);
    if (nbytes < 0)
        printf ("recv() failed: %s\n", strerror (errno));
    else
        printf ("recv() returned %d bytes\n", (int)nbytes);

    rc = close (sock);
    if (rc < 0)
        printf ("close() failed: %s\n", strerror (errno));
    exit (0);
}

