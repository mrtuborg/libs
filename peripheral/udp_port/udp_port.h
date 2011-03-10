#ifndef UDP_PORT_H
#define UDP_PORT_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

class udp_port {
   struct sockaddr_in ip_addr;
   int sock; //socket handler
   
   fd_set static_rdset, static_wrset, rdset, wrset;

   bool RAW;
   bool binded;

public:
    udp_port(WORD num, bool RAW_mode=false, char* if_name=0);
    ~udp_port(void);
    
//getters:

    WORD get_sock();
    
    BYTE scanIfaces(); // return interfaces quantity;
    errType open_port(bool listen_mode=false);
    errType close_port();
    errType sendData(in_addr remote_ip, BYTE *buf, size_t len);
    errType readData(BYTE* addr, size_t* len, in_addr *ip_from=0, bool peek=false);
    errType udp_async_process(BYTE *event_type, DWORD time_sec, DWORD time_usec);
};

#endif // UDP_PORT_H
