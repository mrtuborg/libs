#ifndef JOB_H
#define JOB_H

#include <netinet/in.h>
#include <rcsLib/rcsCmd/rcsCmd.h>

typedef struct in_addr in_addr_struct;
typedef struct sockaddr_in sock_addr_in_struct;

typedef struct job_type{
        DWORD opId;
        DWORD nextOpId;
        DWORD timeStart;
        DWORD timeLong;
        DWORD serviceIPaddr;
         WORD serviceUdpPort;
} __attribute__((packed)) job_type;

class job{
	public:
		job(const job_type &header, const rcsCmd& cmd);
		job(DWORD id = 0);
		virtual ~job();

		int define_init_service(const sock_addr_in_struct& service_addr);

		BYTE  get_cmd_btFuncId(void);
		const BYTE* get_cmd_paramsPtr(size_t offset = 0);
		WORD  get_cmd_paramsLength(void);

		DWORD get_dwTimeStart();
		DWORD get_dwTimeLong();
		DWORD get_dwOpId();
		DWORD get_dwNextOpId();
		DWORD get_dwIPaddr(struct in_addr *out);
		 WORD get_wUdpPort();

		 rcsCmd& rcscmd();


		virtual size_t size();

		friend std::ostream& operator<< (std::ostream& stream, job &jobRef);
		friend std::istream& operator>>(std::istream& stream,  job &jobRef);


  protected:
		virtual int encode(BYTE*);
		virtual int decode(BYTE*);

	private:
	      job_type jobReference;
	        rcsCmd jobEntity;

	        	  BYTE* work_result;

};

#endif
