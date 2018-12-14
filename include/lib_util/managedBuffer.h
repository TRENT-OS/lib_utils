/*
 *  Managed Buffer add convenience wrapper for using a memory buffer
 *
 *  Copyright (C) 2018, Hensoldt Cyber GmbH
 */

#pragma once


//------------------------------------------------------------------------------
static int is_buffer_in_buffer(const void* innerBuffer,
                               size_t innerBufferLen,
                               const void* outerBuffer,
                               size_t outerBufferLen)
{
    const void* innerBufferEnd = (const void*)( (uintptr_t)innerBuffer +
                                                innerBufferLen);
    // cap buffer at end in case of overflow
    if (innerBufferEnd < innerBuffer)
    {
        innerBufferEnd = (const void*)(-1);
    }

    const void* outerBufferEnd = (const void*)( (uintptr_t)outerBuffer +
                                                outerBufferLen);
    // cap buffer at end in case of overflow
    if (outerBufferEnd < outerBuffer)
    {
        // overflow, cap buffer at end
        outerBufferEnd = (const void*)(-1);
    }

    return ( (innerBuffer >= outerBuffer)
             && (innerBufferEnd <= outerBufferEnd) );
}


//------------------------------------------------------------------------------
static int is_buffer_content_equal(const void* buffer1,
                                   size_t buffer1Len,
                                   const void* buffer2,
                                   size_t buffer2Len)
{
    // Note that memcmp() is not time invariant, it will stop on the first
    // mismatch. Thus this function is not suitable if comparing something
    // to sensitive data (e.g. a PIN, password ....).
    return ( (buffer1Len == buffer2Len)
             && (0 == memcmp(buffer1, buffer2,buffer1Len)) );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


typedef struct {
        void*   buffer;
        size_t  len;
        size_t  usedLen;
} managedBuffer_t;

//------------------------------------------------------------------------------
static void managedBuffer_init(managedBuffer_t* managedBuffer,
                              void* buffer,
                              size_t bufferLen)
{
    Debug_ASSERT( NULL != buffer );
    // check that the buffer does no roll over memory boundaries
    Debug_ASSERT( (void*)( (uintptr_t)buffer + bufferLen) > buffer );

    managedBuffer->buffer = buffer;

    // ToDo: better cap to end of memory if this rolls over? The assert() above
    //       will not exist in release builds
    managedBuffer->len = bufferLen;

    managedBuffer->usedLen = 0;
}


//------------------------------------------------------------------------------
static size_t managedBuffer_getFreeSpace(managedBuffer_t* managedBuffer)
{
    return managedBuffer->len - managedBuffer->usedLen;
}


//------------------------------------------------------------------------------
static void* managedBuffer_getFreeSpacePtr(managedBuffer_t* managedBuffer)
{
    // there is an assert in managedBuffer_init() that ensure the buffer is
    // does not roll over memory boundaries, so this addition is safe if nobody
    // tampers with the internals.
    return (void*)( (uintptr_t)managedBuffer->buffer + managedBuffer->usedLen);
}


//------------------------------------------------------------------------------
static int managedBuffer_reserveSpace(managedBuffer_t* managedBuffer,
                                        size_t len)
{
    if (managedBuffer_getFreeSpace(managedBuffer) < len)
    {
        return -1;
    }

    managedBuffer->usedLen += len;

    return 0;
}


//------------------------------------------------------------------------------
static int managedBuffer_append(managedBuffer_t* managedBuffer,
                                void* buffer,
                                size_t bufferLen)
{
    Debug_ASSERT( NULL != buffer );

    if (managedBuffer_getFreeSpace(managedBuffer) < bufferLen)
    {
        return -1;
    }

    memcpy(managedBuffer_getFreeSpacePtr(managedBuffer), buffer, bufferLen);
    managedBuffer->usedLen += bufferLen;

    return 0;
}




//------------------------------------------------------------------------------
static int managedBuffer_appendChar(managedBuffer_t* managedBuffer,
                                    char c)
{
    if (managedBuffer_getFreeSpace(managedBuffer) < 1)
    {
        return -1;
    }

    *((char*)managedBuffer_getFreeSpacePtr(managedBuffer)) = c;
    managedBuffer->usedLen += 1;
    return 0;
}
