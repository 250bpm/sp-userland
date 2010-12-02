#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

#include "sp.h"

int main (int argc, char *argv[])
{
    int sock, rc;
    struct sockaddr_sp addr;

    sock = socket (PF_SP, SOCK_PUB, 0);
    if (sock < 0)
        printf ("socket() failed: %s\n", strerror (errno));
    else
        printf ("Success.");

    addr.ssp_family = AF_SP;
    strcpy (addr.ssp_endpoint, "dummy");
    rc = connect (sock, (struct sockaddr *)&addr, sizeof addr);
    if (rc < 0)
        printf ("connect() failed: %s\n", strerror (errno));

    ssize_t nbytes;
    nbytes = send (sock, "Hello", 5, 0);
    if (nbytes < 0)
        printf ("send() failed: %s\n", strerror (errno));
    else if (nbytes != 5)
        printf ("send() sent only %d bytes\n", (int)nbytes);

    rc = close (sock);
    if (rc < 0)
        printf ("close() failed: %s\n", strerror (errno));
    exit (0);
}

