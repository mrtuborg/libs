#ifndef ORTS_TYPES_H
#define ORTS_TYPES_H
#include <stddef.h>
#include <stdint.h>

typedef  uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef enum {
		type_ERRTYPE=0x00,
		type_BYTE=0x01,
		type_WORD=0x02,
		type_CHAR=0x03,
		type_DWORD=0x04,
		type_FLOAT=0x05,
		type_DOUBLE=0x06,
		type_QWORD=0x08,
		type_BYTEVECTOR=0x11,
		type_WORDVECTOR=0x12,
		type_CHARVECTOR=0x13
	     } OrtsType;

typedef enum {err_result_ok=0,
              err_result_error=1,
              err_crc_error=2,
              err_not_init=3,
              err_params_decode=4,
              err_params_value=5,
              err_extra=6,
              err_timeout=7,
              err_abort=8,
              err_mem_alloc=9,
              err_sock_error=10,
              err_not_found=11} errType;
              
extern char strErrTypes[][255];
extern BYTE lenOrtsTypes[];

typedef errType (*funcPtr)(void*);
typedef errType (*funcVoid)(void);

typedef struct {
    BYTE value: 1;
} __attribute__ ((packed)) booleanBit;

typedef union {
    struct {
	BYTE value_0:1;
	BYTE value_1:1;
    } fields;
    
    BYTE value: 2;
} __attribute__ ((packed)) boolean2Bits;

#endif // ORTS_TYPES_H
