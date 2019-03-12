/// network.c
/// Defines the interface for network.h
/// Network operations like changing hostname -> ip
/// Justin Sostre

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include "chess_network.h"
#include "protocol.h"

int hostname_to_ip(char *hostname, char *ip) {
    struct hostent *h;
    struct in_addr **addr_list;

    if ((h = gethostbyname(hostname)) == NULL) { // if it doesn't exist, 
                                                 // return HOST_INVALID
        return HOST_INVALID;
    }

    addr_list = (struct in_addr **) h->h_addr_list;

    for (int i = 0; addr_list[i] != NULL; i++) { // appends ip
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return HOST_EXISTS;
    }

    return HOST_INVALID;
}
