#define ACTION_SEND 0
#define ACTION_RECEIVE 1

class udpAction
{
    BYTE actionType; // 1-listening, 0-sending
    struct in_addr remote_ip;
    
    pthread_t listenerThread;
    
    udp_port *udpPort;
    rcsCmd *Command;
    
    
    public:
	udpAction(BYTE type, WORD port, char* ip="127.0.0.1");
	~udpAction();
	
	errType writeData(BYTE *data_in, DWORD length);
	errType writeDataAsCmd (rcsCmd *data);
	errType  readData(BYTE **data_out, DWORD *length);
	errType  readDataAsCmd(rcsCmd **data_out);
	
	errType receiveEvent(DWORD timeOut_sec=0, DWORD timeOut_ms=0);
	errType waitRecvEvent();
	errType sendAction();
	
	errType processAction();
	bool isCmdExists();
	const void* getParamPtr(WORD offset=0);
	DWORD getParamLength();
};

extern void* udpListenerThread (void* user);
