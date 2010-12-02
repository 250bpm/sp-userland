#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

#include "sp.h"

int main (int argc, char *argv[])
{
    int sock, rc;

    sock = socket (PF_SP, SOCK_PUB, 0);
    if (sock < 0)
        printf ("socket() failed: %s\n", strerror (errno));
    else
        printf ("Success.");

    rc = close (sock);
    if (rc < 0)
        printf ("close() failed: %s\n", strerror (errno));
    exit (0);
}

