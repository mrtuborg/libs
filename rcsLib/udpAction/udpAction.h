#define ACTION_SEND 0
#define ACTION_RECEIVE 1

class udpAction
{
    BYTE actionType; // 1-listening, 0-sending
    struct in_addr remote_ip;
    
    pthread_t listenerThread;
    
    udp_port *udpPort;
    rcsCmd *Command;
    
    DWORD timeOut_sec;
    DWORD timeOut_ms;


    
    public:
	udpAction(BYTE type, WORD port, const char* ip="127.0.0.1", DWORD setTimeOut_sec=0, DWORD setTimeOut_ms=0);
	~udpAction();
	
	errType receiveEvent();

	errType writeData(BYTE *data_in, WORD length);
	errType writeDataAsCmd (rcsCmd *data);
	errType  readData(BYTE **data_out, WORD *length);
	errType  readDataAsCmd(rcsCmd **data_out);
	
	//errType receiveEvent(DWORD timeOut_sec=0, DWORD timeOut_ms=0);

	errType waitRecvEvent();
	errType sendAction();
	
	errType processAction();
	bool isCmdExists();
	const void* getParamPtr(WORD offset=0);
	DWORD getParamLength();


};

extern void * udpListenerThread (void* user);
