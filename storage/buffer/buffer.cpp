/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class @ref buffer implementation
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
#include "extra/ortsTypes/ortsTypes.h"
#include "buffer.h"

/****************************************//**
 *  Constructs buffer as bytes array of \b size.
 ********************************************/
buffer::buffer(DWORD size)
{
    buff=new BYTE[size];
    WrRef=0;
    RdRef=0;
    buffSize=size;
    writingLock=false;
}


buffer::~buffer()
{
    WrRef=0;
    RdRef=0;
    writingLock=false;
    delete buff;
}

/****************************************************************************//**
 *  @brief Returns length of stored data
 *
 *  @details length is writing reference index minus reading reference index
 *
 *  @retval length - stored data length in bytes
 ********************************************************************************/
DWORD buffer::length()
{
    return WrRef-RdRef;
}

/*****************************************************************************//**
 * Try to write to array data from \b addr pointer with \b len bytes size
 * @retval err_result_ok - writing was succeeded
 * @retval err_result_error - not enough space. Data was written partially.
 *********************************************************************************/
errType buffer::write(BYTE* addr, DWORD len)
{
    errType result=err_result_ok;
    if (WrRef+len>buffSize)
	{
	    result=err_result_error;
	    len=buffSize-len;
	}	
	WrRef+=len;
	memcpy(buff+WrRef-len,addr,len);
	
    return result;
}

/******************************************************************************//**
 * @brief Try to read from array data to \b addr pointer with \b len bytes size.
 *
 * @details If \b len not specified (or equal to 0) method reads all array.
 * @details Readed data has been deleted in buffer.
 * @see buffer::copyBufferChunkTo
 *
 * @retval length - size in bytes of readed data.
 *********************************************************************************/
DWORD buffer::read (BYTE* addr, DWORD len)
{
    DWORD length=WrRef-RdRef;
    if (RdRef+len>WrRef)
    {
	len=WrRef-RdRef;
	//result=err_result_error;
    }
    
    if (len==0)
    {
	len = WrRef-RdRef;
    }
    
    memcpy(addr,buff+RdRef,len);
    RdRef+=len;

    //for (int i=0; i<len; i++) printf("%.2X ",addr[i]);
    //printf("\n");
    if ((!writingLock)&&(WrRef==RdRef)) {
//	printf("flush!\n");
	WrRef=RdRef=0;
    }
    
    return length;
}

/********************************************************************************//**
 *  @brief Allow direct mode writing for external methods.
 *
 *  @details While direct mode writing is locking array - nobody can't to write.
 *  @see Unlock with buffer::unlockBufferChunkForExternWriting.
 *
 *  @retval ptr - pointer to array for writing data
 ************************************************************************************/
BYTE* buffer::lockBufferChunkForExternWriting()
{
     writingLock=true;
     return buff+WrRef;
}


/***************************************************************************************************************//**
 *  @brief Disable direct mode writing from external methods.
 *
 *  @param[in]  offset - length of writing bytes by external methods
 *
 *  @retval err_result_ok - unlocking was successfull
 *  @retval err_result_error - writing bytes was more than space in buffer. Data unlocked to buffer partially.
 *******************************************************************************************************************/
errType buffer::unlockBufferChunkForExternWriting(DWORD offset)
{
    errType result=err_result_ok;
    if ((WrRef+offset)>buffSize) {
	printf("unlockBufferChunkForExternWriting): (WrRef+offset)>buffSize\n");
	printf("WrRef=%d, RdRef=%d, offset=%d, buffSize=%d\n", WrRef, RdRef, offset, buffSize);
	offset=WrRef-offset;
	result=err_result_error;
    }
    writingLock=false;
    WrRef+=offset;
    
    return result;
}

/***********************************************************************************************************//**
 *  @brief    Remove data from buffer
 *  @details Removes data from buffer array, backward writing reference to \b backward_offset and move data
 *            from \b backward_offset + \b len to current writing referense position.
 *  @retval   err_result_ok - removing was successful.
 *  @retval   err_result_error - cannot remove data length more than data size in buffer
 **************************************************************************************************************/
errType buffer::removeBufferChunk(DWORD backward_offset, DWORD len)
{
    errType result=err_result_ok;
    
    if ((WrRef-backward_offset)<RdRef) result=err_result_error;
    else {
	memcpy(buff+(WrRef-backward_offset),buff+(WrRef-backward_offset)+len,len);
	WrRef=WrRef-backward_offset;
    }
    
    return result;
}

/***********************************************************************************************//**
 *  @brief     Try to read from array \b offset data to \b addr pointer with \b len bytes size.
 *
 *  @details  Extract buffer data without deleting it in buffer.
 *  @see       buffer::read
 *
 *  @retval    err_result_ok  -  successfully copied.
 *  @retval    err_result_error - start of block (\b offset) is greater than stored data length.
 **************************************************************************************************/
errType buffer::copyBufferChunkTo(BYTE* addr, DWORD offset, DWORD len)
{
    errType result=err_result_ok;
    printf("copyBufferChunkTo\n");
    if (len==0) len=WrRef-RdRef;
    if ((RdRef+offset)>WrRef) result=err_result_error;
    else {
	memcpy(addr,buff+RdRef+offset,len);
    }
    return result;
}

/***********************************************************************************//**
 * @brief Print for debug purposes contents of stored buffer (in a hexadecimal notation)
 ***************************************************************************************/
errType buffer::dbgPrint()
{
    DWORD i;
    for(i=RdRef;i<WrRef;i++) printf("%.2X ",buff[i]);
    return err_result_ok;
}
