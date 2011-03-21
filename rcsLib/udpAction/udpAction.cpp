#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include <extra/ortsTypes/ortsTypes.h>
#include <peripheral/udp_port/udp_port.h>
#include <rcsLib/rcsCmd/rcsCmd.h>
#include "udpAction.h"

void * udpListenerThread(void* user)
{
    errType result=err_not_init;
    bool terminate=false;
    udpAction *app=(udpAction*)user;
    
//    if (verbose_level) printf("recvEvent: udpListenerThread started\n");
    
    while (!terminate)
    {
        result=app->receiveEvent();
        if ((result==err_timeout) || (result==err_result_ok)) terminate=true;
        sched_yield();
    }
    
/*    if (verbose_level){
	if (result==err_timeout) printf("udpListenerThread exited by timeout\n");
	else printf("udpListenerThread exited by received data event\n");
    }*/

    return user;
}


udpAction::udpAction(BYTE type, WORD port, char* ip, DWORD setTimeOut_sec, DWORD setTimeOut_ms)
{
    actionType=type;
    udpPort=new udp_port(port);
    remote_ip.s_addr=inet_addr(ip);
    Command=new rcsCmd;
    //dataLen=0;
    this->timeOut_ms=setTimeOut_ms;
    this->timeOut_sec=setTimeOut_sec;
}

udpAction::~udpAction()
{
    delete udpPort;
    delete Command;

}

errType udpAction::waitRecvEvent()
{
	errType result=err_not_init;

//	if (verbose_level) printf("Wait for receiving answer\n");
	result=(errType)pthread_join(listenerThread, NULL);
	//printf("ret=%d\n",ret);
	return result;
}

errType udpAction::writeDataAsCmd(rcsCmd *data)
{
    errType result=err_not_init;
    delete Command;

    Command = new rcsCmd(data);

    return result;
}


errType udpAction::writeData(BYTE *data_in, WORD length)
{
    errType result=err_not_init;
    if (length >= MAX_RCS_PARAMS_LENGTH) return err_params_value;
    else {
    		if (Command==0) Command = new rcsCmd;
    		Command->encode(data_in[0],length,data_in+1);
    		result = err_result_ok;
/*    printf("Action: ");
    for (int i=0; i<Command->getCmdLength(); i++) printf("%.2X ",Command->get_func_paramsPtr()[i]);
    printf(", Length=%d\n",Command->getCmdLength());
*/
    }
    return result;
}

errType udpAction::readData(BYTE **data_out, WORD *length)
{
    errType result=err_not_init;
    if (Command == 0 ) result=err_result_error;
    else {
    		Command->decode(*data_out);
    		*length=Command->getCmdLength();
    		result=err_result_ok;
    }
    
    return result;
}

errType udpAction::readDataAsCmd(rcsCmd **data_out)
{
    errType result=err_not_init;
    if (Command == 0 ) result=err_result_error;
    else {
	*data_out=Command;
	result=err_result_ok;
    }
    
    return result;
}


errType udpAction::sendAction()
{
    errType result=err_not_init;
    
    BYTE* data;
    
    if (Command==0) result=err_result_error;
    else {	
	
	udpPort->open_port(actionType); 	//-bool listen_mode=actionType;
	data=new BYTE[Command->getCmdLength()];
	Command->decode(data);
	//---- crc error
	// data[Command->getCmdLength()-2]+=1;
	//---
	result=udpPort->sendData(remote_ip, data, Command->getCmdLength());
	delete []data;
	udpPort->close_port();
    }        
    
    return result;
}

errType udpAction::receiveEvent()
{
    errType result=err_not_init;
    BYTE event=0;
    size_t sz[1];
    *sz=1024; // what if greater?
    

    BYTE* data;
    

    result=udpPort->udp_async_process(&event,timeOut_sec,timeOut_ms);

    if ((result==err_result_ok) & ((event&0x01)==0x01))
    {
    		//printf("Have new data!\n");
    		data=new BYTE[*sz];
    		result=udpPort->readData(data, sz);
	
    		if (Command==0) Command=new rcsCmd;
    		Command->encode(data);
    		delete []data;
    }
    

    return result;
}
      
const void *udpAction::getParamPtr(WORD offset)
{
    return Command->get_func_paramsPtr(offset);
}


bool udpAction::isCmdExists()
{
    if (Command==0) return false;
    else return true;
}

errType udpAction::processAction()
{
    errType result=err_not_init;
    switch (actionType)
    {
    case ACTION_SEND:
	    result=sendAction();
	    break;
    case ACTION_RECEIVE:
	    delete Command;
	    Command=0;
	    
	    udpPort->open_port(true);
	    //int ret=
	    		pthread_create(&listenerThread, NULL, udpListenerThread, (void*) this);
	    waitRecvEvent();
	    
	    udpPort->close_port();
	    if (Command==0) result=err_timeout;
	    else result=err_result_ok;
	    break;
    }
    return result;
    
}

DWORD udpAction::getParamLength()
{
    return Command->get_func_paramsLength();
}
