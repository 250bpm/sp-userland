/*
 * SP: An implementation of SP sockets.
 *
 * Copyright 2011 VMware, Inc.
 */

#ifndef _LINUX_SP_H
#define _LINUX_SP_H

/* For userspace we need to define the AF here */
#define AF_SP           40
#define PF_SP           AF_SP

/* SP socket types */
#define SOCK_PUB        11
#define SOCK_SUB        12
#define SOCK_REQ        13
#define SOCK_REP        14
#define SOCK_PUSH       15
#define SOCK_PULL       16

#define SP_ENDPOINT_MAX   108

struct sockaddr_sp {
	sa_family_t ssp_family; /* AF_SP */
	char ssp_endpoint[SP_ENDPOINT_MAX];  /* endpoint name */
};

#endif /* _LINUX_SP_H */
