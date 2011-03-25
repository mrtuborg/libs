#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#define MAX_SIZE_OF_UDP_DATAGRAM 512

#include "../my_types.h"

class comm_interface
{
public:
    comm_interface(void);
    virtual ~comm_interface(void)                     =0;
    virtual errType open_port(bool listen_mode=false) =0;
    virtual errType close_port()                      =0;
    virtual errType sendByte(BYTE in)                 =0;
    virtual errType readByte(BYTE* out)               =0;
    virtual errType sendData(BYTE* data, size_t len)  =0;
    virtual errType readData(BYTE* data, size_t& len) =0;
    virtual void setReadInterval(WORD dsec)           =0;
    virtual WORD getReadInterval()                    =0;
    virtual int getFD(void)                           =0;
    virtual connection_type getInterfaceType()        =0;
};

#endif // COMM_INTERFACE_H
