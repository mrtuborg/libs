/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class @ref buffer interface header
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

/**
 * @class buffer
 * @brief Simple queue of bytes
 */

class buffer {
	BYTE* buff;
	DWORD WrRef, RdRef, buffSize;
	
	bool writingLock;

	public:
	buffer(DWORD size);
	~buffer();
	
	errType write(BYTE* addr, DWORD len);
	DWORD read (BYTE* addr, DWORD len=0);
	DWORD length();
	
	BYTE* lockBufferChunkForExternWriting();
	errType unlockBufferChunkForExternWriting(DWORD offset);
	errType removeBufferChunk(DWORD backward_offset, DWORD len);
	errType copyBufferChunkTo(BYTE* addr, DWORD offset=0, DWORD len=0);
	//errType copyBufferChunkTo(BYTE* addr, WORD offset=0, BYTE len=0);
	
	errType dbgPrint();
};
