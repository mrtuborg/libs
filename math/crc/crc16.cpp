#include <stdio.h>
#include "extra/ortsTypes/ortsTypes.h"

WORD CRC16_eval_step (WORD crc, BYTE data)
{
    crc = crc ^ ((WORD)data << 8);
    for (int i=0; i<8; i++)
    {
        if (crc & 0x8000)    crc = (crc << 1) ^ 0x1021;
        else               crc <<= 1;
    }
    return crc;
}


WORD CRC16_eval(const void *addr, int len, WORD def_crc=0xFFFF)
{
    WORD crc = def_crc;
    for(int i=0; i<len; i++) crc = CRC16_eval_step(crc, ((BYTE*)addr)[i]);

    return crc;
}
