typedef struct in_addr in_addr_struct;

typedef struct job_type{
        DWORD objId;
        DWORD nextObjId;
        DWORD timeStart;
        DWORD timeLong;
        DWORD serviceIPaddr;
        WORD serviceUdpPort;
        BYTE service_id;
} __attribute__((packed)) job_type;

class job{
      job_type* jobReference;
        rcsCmd* jobEntity;

          BYTE* workResult;
          WORD workResultLength;
          BYTE  state; // 0 - initialized, 1 - running, 2 - completed

	public:
	 job(DWORD id, DWORD serviceAddr, WORD serviceUdpPort);
	~job();
    
	void encode(BYTE*);
	void decode(BYTE*);

	errType set_dwNextJobID(DWORD id);
	errType set_dwStartTime(DWORD time);
	errType set_dwLongTime(DWORD time);
	errType set_btServiceId(BYTE id);
	errType	setJobCmd(BYTE func_id, WORD param_len, void* args);
	errType	setJobCmd(BYTE* cmd);

	WORD getLength();
	
	DWORD get_dwObjId();
	DWORD get_dwNextObjId();
	WORD get_dwTimeStart();
	WORD get_dwTimeLong();
	BYTE get_btServId();
	BYTE get_btFuncId();

	void get_dwServiceIPaddr(struct in_addr *ip);
	WORD get_wServiceUdpPort();

	const void* get_paramsPtr(DWORD offset=0);
	WORD get_paramsLength();

	BYTE getState();
	void lastAnswer(BYTE**, WORD*);

	void setState(BYTE newState);
	void setAnswer(BYTE* setAnswer, WORD length);

	rcsCmd* cmd();
	
	void dbgPrint();
};
