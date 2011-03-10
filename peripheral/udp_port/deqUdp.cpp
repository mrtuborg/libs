/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class deqUdp implementation
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <storage/ssBuffer/ssBuffer.h>
#include <comm/udp_port/udp_port.h>
#include "deqUdp.h"


//deqUdp::deqUdp(WORD portNum, const char* ip_str):udp_port(portNum, ip_str)
//{
//
//}

deqUdp::~deqUdp()
{

}

/******************************************************************//**
 * @brief       Send data to udp port from queued buffer (ssBuffer)
 * @param[in]   buf - pointer to queue for sending data
 * @retval      err_result_ok - execution was successful
 * @retval      err_result_error - problems with udp sendto function
 **********************************************************************/
errType deqUdp::sendData(ssBuffer *buf)                                                                                                                                 
{
  /// @todo not needed to realize all sendData check ability to use base method sendData of parent class
//    if (verbose_level) printf("udp_port::sendData()\n");                                                                                                                    
    int flags=0;                                                                                                                                                            
    errType result=err_result_ok;                                                                                                                                           
    sockaddr_in host_ip;                                                                                                                                                    
    WORD sock=get_sock();
    BYTE *sendData;                                                                                                                                                         
    DWORD len=buf->getFrontBlockSize();                                                                                                                                     
    memset(&host_ip,0,sizeof(host_ip));                                                                                                                                     
    sendData=new BYTE[len];                                                                                                                                                 
    buf->popBlock(&host_ip, sendData);                                                                                                                                      
    printf("Отправка пакета: содержимое [");                                                                                                                                
    for (size_t i=0; i<len; i++) printf("%.2X ", sendData[i]);                                                                                                              
    printf("]\n\t\t\t адресат: %s, сокет %d/udp\n\n",inet_ntoa(host_ip.sin_addr),ntohs(host_ip.sin_port));                                                                  
    ssize_t sended=0;                                                                                                                                                       
    sended=sendto(sock, sendData, len, flags,(sockaddr*) &host_ip, sizeof(host_ip));                                                                                        
//    if (verbose_level) printf("data length=%d want to be sent via udp port %d, sended=%d\n",(int)len,ntohs(ip_addr.sin_port),(int)sended);                                  
    if (sended==-1)                                                                                                                                                         
    {                                                                                                                                                                       
//	if (verbose_level) printf("errno=%d\n",errno);                                                                                                                      
	    result=err_result_error;                                                                                                                                            
    }                                                                                                                                                                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    delete []sendData;
    return result;                                                                                                                                                          
}

/******************************************************************//**
 * @brief       Read udp port data to queued buffer (ssBuffer)
 * @param[in]   addr - pointer to queue for readed data
 * @param[in]   peek - PEEK MODE
 * @param[out]  len - length of readed data
 * @param[out]  ipaddr - information about data sender
 * @retval      err_result_ok - execution was successful
 * @retval      err_result_error - problems with udp sendto function
 **********************************************************************/
errType deqUdp::readData(ssBuffer* addr, size_t* len, in_addr *ipaddr,bool peek)
{
  /// @todo not needed to realize all sendData check ability to use base method sendData of parent class
	//    if (verbose_level) printf("udp_port::readData(buffer)\n");
    errType result=err_result_ok;
    int numbytes=0;
    socklen_t socklen=0;
    sockaddr_in sfrom;//=new sockaddr_in;
    bool repeat=false;
    WORD sock=get_sock();
    
    
    BYTE tmp_rcvData[*len];
    //BYTE* rcvData;
        
    socklen=sizeof(sockaddr_in);
    memset(&sfrom,0,sizeof(sockaddr_in));
    sfrom.sin_family=AF_INET;
    
    
//    if (verbose_level) printf("listening %lu bytes on %d port\n",*len,ntohs(ip_addr.sin_port));
    do {
        if (peek) numbytes = recvfrom(sock, tmp_rcvData, *len , MSG_PEEK,(struct sockaddr *)&sfrom, &socklen);
        else numbytes = recvfrom(sock, tmp_rcvData, *len , 0 ,(struct sockaddr *)&sfrom, &socklen);


        //printf("packet from %s:%d\n",inet_ntoa(sfrom.sin_addr),ntohs(sfrom.sin_port));
        if (((sfrom.sin_addr.s_addr)&0xFF000000)==0) repeat=true;
    } while (repeat); 
    
    if ( numbytes == -1) {
        perror("recvfrom");
        result=err_result_error;

    } else if (numbytes>0) {

        *len=numbytes;
        addr->pushBlock(&sfrom, tmp_rcvData, numbytes);
//		if (verbose_level) addr->dbgPrint();
//
    }
    return result;
}

