typedef struct job_type{
        DWORD objId;
        DWORD nextObjId;
        DWORD timeStart;
        DWORD timeEnd;
        DWORD serviceIPaddr;
        WORD serviceUdpPort;
        BYTE service_id;
} __attribute__((packed)) job_type;

class job{
      job_type* jobReference;
        rcsCmd* jobEntity;


	public:
	 job(DWORD id, DWORD serviceAddr, WORD serviceUdpPort);
	~job();
    
	void encode(BYTE*);
	void decode(BYTE*);

	errType set_dwNextJobID(DWORD id);
	errType set_dwStartTime(DWORD time);
	errType set_dwFinishTime(DWORD time);
	errType set_btServiceId(BYTE id);
	errType	setJobCmd(BYTE func_id, WORD param_len, void* args);
	errType	setJobCmd(BYTE* cmd);

	WORD getLength();
	
	DWORD get_dwObjId();
	DWORD get_dwNextObjId();
	WORD get_dwTimeStart();
	WORD get_dwTimeEnd();
	BYTE get_btServId();
	BYTE get_btFuncId();

	DWORD get_serviceIPaddr();
	WORD get_serviceUdpPort();

	const void* get_paramsPtr(DWORD offset=0);
	WORD get_paramsLength();


	rcsCmd* cmd();
	
	void dbgPrint();
};
