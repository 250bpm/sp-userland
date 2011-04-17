/*
 * SP: An implementation of SP sockets.
 *
 * fanout_client: An example client which receives messages from pubsub_server.
 *
 * Author: Martin Lucina <mato@kotelna.sk>, Martin Sustrik <sustrik@250bpm.com>
 * Copyright 2011 VMware, Inc.
 */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include "sp.h"

int main (int argc, char *argv[])
{
        int sock, rc, pid;
        struct sockaddr_sp addr;
        char buffer[1024];
        ssize_t nbytes;

        if (argc < 2) {
                fprintf (stderr, "usage: reqrep_client ENDPOINT\n");
                exit (1);
        }

        /* Create an SP socket. */
        sock = socket (PF_SP, SOCK_REQ, 0);
        if (sock < 0) {
                fprintf (stderr, "socket() failed: %s\n", strerror (errno));
                exit (1);
        }

        /* Connect it to the supplied ENDPOINT (e.g. tcp://127.0.0.1:1111) */
        memset (&addr, 0, sizeof addr);
        addr.ssp_family = AF_SP;
        strcpy (addr.ssp_endpoint, argv[1]);
        rc = connect (sock, (struct sockaddr *)&addr, sizeof addr);
        if (rc < 0) {
                fprintf (stderr, "connect() failed: %s\n", strerror (errno));
                exit (1);
        }

        /* Send a request. The request contains pid of the client. */
        pid = (int) getpid ();
        sprintf (buffer, "%d", pid);
        printf ("sending <%s>\n", buffer);
        nbytes = send (sock, buffer, strlen (buffer), 0);
        if (nbytes < 0) {
                fprintf (stderr, "send() failed: %s\n",
                        strerror (errno));
                exit (1);
        }

        /* Receive the reply */
        nbytes = recv (sock, buffer, sizeof buffer, 0);
        if (nbytes < 0) {
                fprintf (stderr, "recv() failed: %s\n",
                        strerror (errno));
                exit (1);
        }

        /* Print the content as a string */
        buffer[nbytes] = 0;
        printf ("received <%s>\n", buffer);

        /* Clean up */
        rc = close (sock);
        if (rc < 0) {
                fprintf (stderr, "close() failed: %s\n", strerror (errno));
                exit (1);
        }
        exit (0);
}



