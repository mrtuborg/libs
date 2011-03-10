/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class ssBuffer interface header
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */
using namespace std;

/**
 * @struct ssBlock
 * @brief \ref ssBuffer list entry.
 */
typedef struct ssBlock
{
    sockaddr_in addr_in; ///< recipient of datablock address,
	DWORD dataLen;       ///< size in bytes of data,
	BYTE* data;          ///< data array pointer.

} ssBlock;

/**
 * @class ssBuffer
 * @brief list (deque) implementaion for storing ssBlock elements.
 * @details deque uses to organize sending or receiving queue
 */
class ssBuffer
{
    deque <ssBlock*> buffer;
    
    public:
    
     ssBuffer();
    ~ssBuffer();
    
    errType pushBlock(sockaddr_in* , BYTE*, DWORD len);
    DWORD  popBlock(sockaddr_in* , BYTE*);
    
    errType getBlockPtrAt(int index, ssBlock* block);
    DWORD getFrontBlockSize();
    
    DWORD size();
    
    void dbgPrint();
};
