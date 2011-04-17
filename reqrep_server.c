/*
 * SP: An implementation of SP sockets.
 *
 * fanout_server: An example server which sends messages to one or more
 * instances of pubsub_client.
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
        int sock, rc;
        struct sockaddr_sp addr;
        char buffer[1024];
        ssize_t nbytes;

        if (argc < 2) {
                fprintf (stderr, "usage: reqrep_server ENDPOINT\n");
                exit (1);
        }

        /* Create an SP socket. */
        sock = socket (PF_SP, SOCK_REP, 0);
        if (sock < 0) {
                fprintf (stderr, "socket() failed: %s\n", strerror (errno));
                exit (1);
        }

        /* Bind it to the supplied ENDPOINT (e.g. tcp://0.0.0.0:1111) */
        memset (&addr, 0, sizeof addr);
        addr.ssp_family = AF_SP;
        strcpy (addr.ssp_endpoint, argv[1]);
        rc = bind (sock, (struct sockaddr *)&addr, sizeof addr);
        if (rc < 0) {
                fprintf (stderr, "bind() failed: %s\n", strerror (errno));
                exit (1);
        }

        while (1) {

                /* Receive a request */
                nbytes = recv (sock, buffer, sizeof(buffer), 0);
                if (nbytes < 0) {
                        fprintf (stderr, "recv() failed: %s\n",
                                strerror (errno));
                        exit (1);
                }

                /* Print the content as a string */
                buffer[nbytes] = 0;
                printf ("processing <%s>\n", buffer);

                /* Send the reply. The content of the reply is the same as
                   content of the original request. */
                nbytes = send (sock, buffer, nbytes, 0);
                if (nbytes < 0) {
                        fprintf (stderr, "send() failed: %s\n",
                                strerror (errno));
                        exit (1);
                }
        }

        /* Not reached. Code to close the socket follows if the above loop
           were to exit. */
        rc = close (sock);
        if (rc < 0) {
                fprintf (stderr, "close() failed: %s\n", strerror (errno));
                exit (1);
        }
        exit (0);
}

