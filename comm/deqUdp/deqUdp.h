/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class deqUdp interface header
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

/**
 * @class deqUdp
 * @brief udp communications (based on udp_port) with queues for listening and sending
 */

class deqUdp: public udp_port
{
    


    public:
    
    deqUdp(WORD portNum, const char* ip_str="127.0.0.1"):udp_port(portNum, ip_str){};
    ~deqUdp();

    errType sendData(ssBuffer *buf);

    errType readData(ssBuffer* addr, size_t* len, in_addr *ip_from=0, bool peek=false);                                                                                   

};
