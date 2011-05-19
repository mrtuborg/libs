typedef struct in_addr in_addr_struct;

typedef struct job_type{
        DWORD opId;
        DWORD nextOpId;
        DWORD timeStart;
        DWORD timeLong;
        DWORD serviceIPaddr;
         WORD serviceUdpPort;
        DWORD answerIPaddr;
         WORD answerUdpPort;
        DWORD mask_offset;
        DWORD mask_len;
        DWORD mask_def;
} __attribute__((packed)) job_type;

class job{
      job_type* jobReference;
        rcsCmd* jobEntity;
        rcsCmd* ansEntity;
          BYTE* workResult;
          WORD  workResultLength;
          BYTE  state; // 0 - initialized, 1 - running, 2 - completed

	public:
     job();
	/* job(DWORD id, DWORD serviceAddr   ,  WORD serviceUdpPort,
	 	           DWORD answerAddr = 0,  WORD answerUdpPort = 0 );*/
	~job();
    
	void encode(BYTE*);
	void decode(BYTE*);

	errType set_dwNextJobID(DWORD id);
	errType set_dwStartTime(DWORD time);
	errType set_dwLongTime(DWORD time);
	errType	setJobCmd(BYTE func_id, WORD param_len, void* args);
	errType	setJobCmd(BYTE* cmd);

	errType	setAnsCmd(BYTE func_id, WORD param_len, void* args);
	errType	setAnsCmd(BYTE* cmd);

	WORD size();
	
	DWORD get_dwOpId();
	DWORD get_dwNextOpId();
	DWORD get_dwTimeStart();
	WORD get_dwTimeLong();

	BYTE get_btFuncId();
	BYTE get_ans_btFuncId();

	void get_dwServiceIPaddr(struct in_addr *ip);
	WORD get_wServiceUdpPort();

	void get_dwAnswerIPaddr(struct in_addr *ip);
	WORD get_wAnswerUdpPort();

	const void* get_paramsPtr(DWORD offset=0);
	WORD get_paramsLength();

	const void* get_ans_paramsPtr(DWORD offset=0);
	WORD get_ans_paramsLength();


	BYTE getState();
	void lastAnswer(BYTE**, WORD*);

	void setState(BYTE newState);
	void setAnswer(BYTE* setAnswer, WORD length);

	BYTE checkAnswerByMask();

	rcsCmd* cmd();
	rcsCmd* ans_cmd();

	void dbgPrint();

	void set_mask (DWORD offset, DWORD len, DWORD def);
};
