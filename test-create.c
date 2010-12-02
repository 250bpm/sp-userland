#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

#include "sp.h"

int main (int argc, char *argv[])
{
    int sock;

    sock = socket (PF_SP, 0, 0);
    if (sock < 0)
        printf ("socket() failed: %s\n", strerror (errno));
    else
        printf ("Success.");

    close (sock);
    exit (0);
}

