#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <queue>
#include "extra/ortsTypes/ortsTypes.h"
#include "udp_port.h"

BYTE udp_port::scanIfaces() {
    struct ifaddrs * ifAddrStruct=NULL, *ifAddrStruct_head=NULL;
    struct in_addr* tmpAddrPtr=NULL;
    BYTE ifCount=0;
    
//    if (verbose_level) printf("udp_port::scanIfaces()\n");
    
    if (getifaddrs(&ifAddrStruct)==-1) {
	printf("getifaddrs error!\n");
	return 0;
    }
	
	
	ifAddrStruct_head=ifAddrStruct;
	//printf("---------------------------------------------\n");
	//printf("  interfaces: ");
                                                                      
	while (ifAddrStruct!=NULL) {
	    if (ifAddrStruct->ifa_addr->sa_family==AF_INET) {
		tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
		//some bugs here:printf("IP Address %s\n", inet_ntop(AF_INET, tmpAddrPtr, addressBuffer,sizeof(sockaddr_in)));
		//printf("[ %s ] ",ifAddrStruct->ifa_name);
		ifCount++;
	    //if (!strncmp(ifAddrStruct->ifa_name,if_name,3)) // cmp with "eth0", "eth1"....
	    }
	    ifAddrStruct=ifAddrStruct->ifa_next;
	}
                      
    freeifaddrs(ifAddrStruct_head);
    return ifCount;
}

udp_port::udp_port(WORD num, bool RAW_mode, char* if_name)
{
//   if (verbose_level) printf("udp_port::udp_port()\n");
    
    RAW=RAW_mode;

    memset(&ip_addr,0,sizeof(ip_addr));
    
    if (if_name==0) {
	ip_addr.sin_family = AF_INET;
	ip_addr.sin_port = htons(num);
	ip_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    } else {
	printf("SCAN INTERFACES\n");
	    //scanIfaces(num);
    }	

//    if (verbose_level) printf ("set udp_socket interface: port=%d, ip_addr=[%.8X]\n", ntohs(ip_addr.sin_port), ip_addr.sin_addr.s_addr);
    binded=false;
 
}

//getters:
WORD udp_port::get_sock() { return sock; }





udp_port::~udp_port(void)
{
//    if (verbose_level) printf("udp_port:~udp_port()\n");
}

errType udp_port::open_port(bool listen_mode)
{
//    if (verbose_level) printf("udp_port::open_port()\n");
    errType result=err_result_ok;

    if (listen_mode) {
//        if (verbose_level) printf("listening via %d/udp port\n",ntohs(ip_addr.sin_port));

        sock = socket(AF_INET, SOCK_DGRAM, 0);
//            if (verbose_level) printf("Create datagramm socket.Socket #%d\n",sock);
	//    printf("pt2\n");

    
        //     printf("pt2\n");

        result=(errType)bind(sock, (struct sockaddr*)&ip_addr, sizeof(ip_addr));
        if (result==err_result_ok) {
            binded=true;
            //ioctl (sock, SIO_RCVALL, &flag);
        }
        else {
//            if (verbose_level)  printf ("bind fails");
//            if (verbose_level)  printf (",errno=%d\n",errno);
            //perror(errno);
            result=err_not_init;
        }
    } else {
//        if (verbose_level) printf("sending via %d/udp port\n",ntohs(ip_addr.sin_port));
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    }

    FD_ZERO(&static_rdset);
    FD_ZERO(&static_wrset);

    if (listen_mode) FD_SET(sock, &static_rdset);
    if (!listen_mode) FD_SET(sock, &static_wrset);

    return result;
}

errType udp_port::close_port()
{
//    if (verbose_level) printf("udp_port::close_port()\n");
    errType result=err_result_ok;
    
    FD_ZERO(&static_rdset);
    FD_ZERO(&static_wrset);

    close(sock);

    return result;
}


/*errType udp_port::sendData(ssBuffer *buf)
{
    if (verbose_level) printf("udp_port::sendData()\n");
    int flags=0;
    errType result=err_result_ok;
    sockaddr_in host_ip;
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

    if (verbose_level) printf("data length=%d want to be sent via udp port %d, sended=%d\n",(int)len,ntohs(ip_addr.sin_port),(int)sended);

    if (sended==-1)
    {
        if (verbose_level) printf("errno=%d\n",errno);
        result=err_result_error;
    }

    delete sendData;
    return result;
}


  */
errType udp_port::sendData(in_addr remote_ip, BYTE *buf, size_t len)
{
//    if (verbose_level) printf("udp_port::sendData()\n");
    int flags=0;
    errType result=err_result_ok;
    sockaddr_in host_ip;
    memset(&host_ip,0,sizeof(host_ip));
    host_ip.sin_family=AF_INET;
    host_ip.sin_addr=remote_ip;
    host_ip.sin_port=ip_addr.sin_port;

    ssize_t sended=0;

    printf("Отправка пакета: содержимое [");
    for (size_t i=0; i<len; i++) printf("%.2X ", buf[i]);
    printf("]\n\t\t\t адресат: %s, сокет %d/udp\n\n",inet_ntoa(remote_ip),ntohs(ip_addr.sin_port));

    sended=sendto(sock,buf,len, flags,(sockaddr*) &host_ip, sizeof(host_ip));

//    if (verbose_level) printf("data length=%d want to be sent via udp port %d, sended=%d\n",(int)len,ntohs(ip_addr.sin_port),(int)sended);

    if (sended==-1)
    {
        printf("errno=%d\n",errno);
        result=err_result_error;
    }

    return result;
}

//  Checks the state of udp_port - ready for read (has datagram) or ready for write
//  The description of parameters:
//    Value of timeout sets the time interval for awaiting event.
//    If there are no events during that time - returns err_timeout. If it is - return err_ok.
//    Variants of timeout values:
//      0 - check current state of port. if no events - return err_timeout immediately
//      > 0 - wait for event appearance during timeout interval. If no events - return err_timeout
//      -1 - unlimited awaiting of event appearance.
//    event_type - returns code of port's state if it has event:
//      1 - port ready for read (has datagram for read)
//      2 - port ready for write (ready for send datagram)
errType udp_port::udp_async_process(BYTE *event_type, DWORD timeout_sec, DWORD timeout_usec)
{
    errType result = err_not_init;

    *event_type = 0;
    int retval  = 0;

    rdset = static_rdset;
    wrset = static_wrset;

    struct timeval timeout_value;
    timeout_value.tv_sec  = timeout_sec;
    timeout_value.tv_usec = timeout_usec;

    if (timeout_sec == (DWORD)-1)
        retval = select(sock+1, &rdset, &wrset, NULL, NULL);
    else
        retval = select(sock+1, &rdset, &wrset, NULL, &timeout_value);

    if (retval == -1) printf("select()\n");
    else if (retval)  {
        if (FD_ISSET(sock, &rdset))  {  // 0000 0001 - read state
            /* do recvform and handle your NAK */
            *event_type |= 0x01;
            result = err_result_ok;
        }

        if (FD_ISSET(sock, &wrset)) {  // 0000 0010 - write state
            /* do sendto */
            *event_type |= 0x02;
            result = err_result_ok;
        }
    } else  {
        result = err_timeout;
    }
    return result;
}


errType udp_port::readData(BYTE* addr, size_t* len, in_addr *ipaddr,bool peek)
{
//    if (verbose_level) printf("udp_port::readData(BYTE*)\n");
    errType result=err_result_ok;
    int numbytes=0;
    socklen_t socklen=0;
    sockaddr_in sfrom;

//    if (verbose_level) printf("listening %lu bytes on %d port\n",*len,ntohs(ip_addr.sin_port));
    if (peek) numbytes = recvfrom(sock, addr, *len , MSG_PEEK,(struct sockaddr *)&sfrom, &socklen);
    else numbytes = recvfrom(sock, addr, *len , 0 ,(struct sockaddr *)&sfrom, &socklen);
    
    if ( numbytes == -1) {
        perror("recvfrom");
        result=err_result_error;

    } else {
    	*len=numbytes;
    	

/*	if (verbose_level) {
    		printf("Received follows: [");
    		for(i=0;i<*len;i++) printf("%.2X ",(addr)[i]);
    		printf("]\n");
    	}*/
    }
    return result;
}
    /*
errType udp_port::readData(ssBuffer* addr, size_t* len, in_addr *ipaddr,bool peek)
{
//    if (verbose_level) printf("udp_port::readData(buffer)\n");
    errType result=err_result_ok;
    int numbytes=0, i=0;
    socklen_t socklen=0;
    sockaddr_in sfrom;//=new sockaddr_in;
    bool repeat=false;
    
    
    BYTE* tmp_rcvData=new BYTE[*len];
    BYTE* rcvData;
    
    struct udphdr *tmp_udphdr;
    BYTE udp_hdr_size=sizeof(struct udphdr);
    
    #ifdef linux
	struct iphdr *tmp_iphdr;
	BYTE ip_hdr_size=sizeof(struct iphdr);
    #else
	struct ip *tmp_iphdr;
	BYTE ip_hdr_size=sizeof(struct ip);
    #endif
    
    socklen=sizeof(sockaddr_in);
    memset(&sfrom,0,sizeof(sockaddr_in));
    sfrom.sin_family=AF_INET;
    
    
    if (verbose_level) printf("listening %lu bytes on %d port\n",*len,ntohs(ip_addr.sin_port));
    do {
	if (peek) numbytes = recvfrom(sock, tmp_rcvData, *len , MSG_PEEK,(struct sockaddr *)&sfrom, &socklen);
	else numbytes = recvfrom(sock, tmp_rcvData, *len , 0 ,(struct sockaddr *)&sfrom, &socklen);
    
	
	//printf("packet from %s:%d\n",inet_ntoa(sfrom.sin_addr),ntohs(sfrom.sin_port));
	if (((sfrom.sin_addr.s_addr)&0xFF000000)==0) repeat=true;
	if (RAW) {
	    
	    #ifdef linux
		tmp_iphdr=(struct iphdr*)tmp_rcvData;
	    #else
		tmp_iphdr=(struct ip*)tmp_rcvData;
	    #endif
	    
	    tmp_udphdr=(struct udphdr*)(tmp_rcvData+ip_hdr_size);
	    
	    #ifdef linux
	    if ((((tmp_iphdr->daddr)&0xFF000000)>>24)==0xFF) {
		repeat=true;
		//printf("Message not for me!\n");
	    }
	    else {
		if (tmp_udphdr->dest!=ip_addr.sin_port) {
		    repeat=true;
		//    printf("Not my message!\n");
		} else {
		    repeat=false;
		//    printf("\n You have a message :)\n");
	        
		}
	    }
	    #endif
	    
	    
	}
    } while (repeat); 
    
    if ( numbytes == -1) {
        perror("recvfrom");
        result=err_result_error;

    } else if (numbytes>0) {
	rcvData=new BYTE[numbytes];
	memcpy(rcvData, tmp_rcvData, numbytes);

	*len=numbytes;
	
	if (RAW) {
	      #ifdef linux
		sfrom.sin_family=AF_INET;
	        sfrom.sin_addr.s_addr=tmp_iphdr->saddr;
	        sfrom.sin_port=tmp_udphdr->dest;
	        numbytes=numbytes-(sizeof(struct iphdr)+sizeof(udphdr));
	        rcvData+=(sizeof(struct iphdr)+sizeof(udphdr));
	        *len=numbytes;
	        
	      #endif
	      
	}
	
	delete tmp_rcvData;
	
	addr->pushBlock(&sfrom, rcvData, numbytes);
	if (verbose_level) addr->dbgPrint();
    
    }
    return result;
}

      */

