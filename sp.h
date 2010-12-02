#ifndef SP_H_INCLUDED
#define SP_H_INCLUDED

#define AF_SP           38
#define PF_SP           AF_SP

#define SOCK_PUB        11
#define SOCK_SUB        12

#define SP_ADDRESS_MAX	108

struct sockaddr_sp {
        sa_family_t ssp_family;                 /* AF_SP */
        char ssp_endpoint[SP_ADDRESS_MAX];      /* Endpoint */
};

#endif
