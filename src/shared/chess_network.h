/// network.h
/// interface for network functions
/// Justin Sostre

#ifndef _CHESS_NET_H
#define _CHESS_NET_H


/// host_to_ip - converts hostname to ip address
/// hostname - hostname (can be ip)
/// ip - pointer to ip address
/// return - error code if does not exist
int hostname_to_ip(char *hostname, char *ip);


#endif
