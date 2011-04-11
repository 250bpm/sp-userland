/*
 * SP: An implementation of SP sockets.
 *
 * Copyright 2011 VMware, Inc.
 */

#ifndef _LINUX_SP_H
#define _LINUX_SP_H

/* For userspace we need to define the AF here */
#define AF_SP           38
#define PF_SP           AF_SP

#define SP_ENDPOINT_MAX   108

struct sockaddr_sp {
	sa_family_t ssp_family; /* AF_SP */
	char ssp_endpoint[SP_ENDPOINT_MAX];  /* endpoint name */
};

#endif /* _LINUX_SP_H */
