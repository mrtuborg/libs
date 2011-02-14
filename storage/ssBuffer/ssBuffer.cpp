/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class ssBuffer implementation
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <string.h>
#include <stdio.h>
#include <deque>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <extra/ortsTypes/ortsTypes.h>
#include "ssBuffer.h"

ssBuffer::ssBuffer()
{
    buffer.clear();
}

ssBuffer::~ssBuffer()
{
    buffer.clear();
}

/*******************************************************************************//**
 * @brief push datablock to queue
 * @details copies data from \b block and \b addr pointers to new memory locations
 * @param[in]   addr  - recepient address (owner of datablock)
 * @param[in]   block - datablock
 * @param[in]   len   - size in bytes of datablock
 * @retval      err_result_ok - pushing was successfully
 **********************************************************************************/
errType ssBuffer::pushBlock(sockaddr_in* addr, BYTE* block, DWORD len)
{
    errType result = err_result_ok;
    ssBlock* dataBlock = new ssBlock;
    //printf("Wanna send to IP: %s\n",inet_ntoa(addr->sin_addr));
    //printf("to udp port: %d\n",ntohs(addr->sin_port));
    memcpy(&dataBlock->addr_in, addr, sizeof(sockaddr_in));
    dataBlock->data = new BYTE[len];
    memcpy(dataBlock->data, block, len);
    dataBlock->dataLen = len;
    buffer.push_back(dataBlock);
    //dbgPrint();
    return result;
}

/*******************************************************************************//**
 * @brief get first in queue (front) block size in bytes
 * @details need to be prepared for reading next queue element
 * @retval      dataLen - size in bytes of data block
 **********************************************************************************/
DWORD ssBuffer::getFrontBlockSize()
{
    ssBlock* dataBlock;
    dataBlock=buffer.front();
    return dataBlock->dataLen;
}

/*******************************************************************************//**
 * @brief pop datablock from queue
 * @details copies data from queue to \b block and \b addr pointers
 * @details \b block and \b addr need to be allocated before calling this method.
 * @details queue element will be removed from queue
 * @param[in]   addr  - recepient address (owner of datablock)
 * @param[in]   block - datablock
 * @retval      lenght - size in bytes of readed datablock
 **********************************************************************************/
DWORD ssBuffer::popBlock(sockaddr_in* addr, BYTE* block)
{
    DWORD len=0;
    ssBlock* dataBlock=0;
    dataBlock=buffer.front();
    if (dataBlock) {
	buffer.pop_front();
	//if (dataBlock->addr_in) {
	    memcpy(addr,&dataBlock->addr_in,sizeof(sockaddr_in));
	    len=dataBlock->dataLen;
	    memcpy(block, dataBlock->data, len);
	//}
    }
    return len;
}

/*******************************************************************************//**
 * @brief read item from queue by \b index
 * @param[in]   index  - queue item index
 * @param[out]   block - readed queue item
 **********************************************************************************/
errType ssBuffer::getBlockPtrAt(int index, ssBlock* block)
{
    errType result=err_result_ok;
    
    block=buffer.at(index);
    
    return result;
}

/******************************************************************************//**
 * @brief return length (items quantity) of queue
 **********************************************************************************/

DWORD ssBuffer::size()
{
    //if (buffer!=0) 
    return buffer.size();
    //else return 0;
}


/***********************************************************************************//**
 * @brief Print for debug purposes contents of stored queue (in a hexadecimal notation)
 ***************************************************************************************/
void ssBuffer::dbgPrint()
{
    ssBlock* dataBlock;
    DWORD k=0, i=1, len=0, q=0;
    
    q=size();
    printf("Blocks quantity=%d\n", q);
    while (i<=q) {
	dataBlock=buffer[i-1];
	len=dataBlock->dataLen;
	printf("Block #%d\n",i);
	printf("IP address: %s\n",inet_ntoa(dataBlock->addr_in.sin_addr));
	printf("UDP port: %d\n",ntohs(dataBlock->addr_in.sin_port));
	printf("Data length: %d\n", len);
	printf("Data :[");
	for (k=0; k<len; k++) printf("%.2X ",dataBlock->data[k]);
	printf("]\n");
	i++;
    } ;
    printf("------------\n");
}

