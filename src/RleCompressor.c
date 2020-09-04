/**
 * @addtogroup LibUtil
 * @{
 *
 * @file RleCompressor.c
 *
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 */

#include "OS_Error.h"

#include "lib_util/BitConverter.h"
#include "lib_util/RleCompressor.h"

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// The worst case length for an encoded symbol is 4 bytes for the length indicator
// and one byte for the symbol itself
#define ENCODED_SYMBOL_MAX_SIZE (sizeof(uint32_t) + 1)
// Length of header we always have, i.e., "RLE" and four lenght bytes indicating
// decompressed length
#define HEADER_LENGTH (sizeof(uint32_t) + 3)

// Private functions -----------------------------------------------------------

static uint8_t*
lenSerializeOpt(
    const size_t len,
    uint8_t*     p)
{
    /*
     * Sligtly optimized serialization routine: We do not need to always use a
     * full 4 bytes to encode the length, if the length does not require that.
     * The first 2 bits of the length field always indicate how many bytes were
     * used in the encoding.
     */

    if (len < (1u << 6))
    {
        *(p++) = 0b00000000 |  (len & 0x3F);
    }
    else if (len < (1u << 14))
    {
        *(p++) = 0b01000000 | ((len >>  8) & 0x3F);
        *(p++) =               (len >>  0) & 0xFF;
    }
    else if (len < (1u << 22))
    {
        *(p++) = 0b10000000 | ((len >> 16) & 0x3F);
        *(p++) =               (len >>  8) & 0xFF;
        *(p++) =               (len >>  0) & 0xFF;
    }
    else
    {
        *(p++) = 0b11000000 | ((len >> 24) & 0x3F);
        *(p++) =               (len >> 16) & 0xFF;
        *(p++) =               (len >>  8) & 0xFF;
        *(p++) =               (len >>  0) & 0xFF;
    }

    return p;
}

static uint8_t*
lenDeserializeOpt(
    const uint8_t* p,
    size_t*        len)
{
    /*
     * Corresponding decoder; always check the first two bits of the length
     * field to figure out how many bytes were used to encode the length
     */

    switch ((*p) >> 6)
    {
    case 0b11:
        *len = (size_t) ((*(p + 0)) & 0x3f) << 24 |
               (size_t)  (*(p + 1))         << 16 |
               (size_t)  (*(p + 2))         <<  8 |
               (size_t)  (*(p + 3))         <<  0;
        return ((uint8_t*)p + 4);
    case 0b10:
        *len = (size_t) ((*(p + 0)) & 0x3f) << 16 |
               (size_t)  (*(p + 1))         <<  8 |
               (size_t)  (*(p + 2))         <<  0;
        return ((uint8_t*)p + 3);
    case 0b01:
        *len = (size_t) ((*(p + 0)) & 0x3f) <<  8 |
               (size_t)  (*(p + 1))         <<  0;
        return ((uint8_t*)p + 2);
    case 0b00:
        *len = (size_t) ((*(p + 0)) & 0x3f) <<  0;
        return ((uint8_t*)p + 1);
    }

    return NULL;
}

static inline OS_Error_t
compressLoop(
    const uint8_t* ip,
    const uint8_t* iBoundary,
    uint8_t* op,
    const uint8_t* oBoundary)
{
    while (ip < iBoundary)
    {
        // Make sure we can fit at least another symbol; we assume it can have
        // maximum size
        if ((op + ENCODED_SYMBOL_MAX_SIZE) >= oBoundary)
        {
            return OS_ERROR_ABORTED;
        }

        uint8_t sym = *ip;
        size_t slen = 1;
        // Check how often the symbol is found in sequence
        while (++ip < iBoundary && *ip == sym)
        {
            slen++;
        }
        op = lenSerializeOpt(slen, op);
        *(op++) = sym;
    }
    return OS_SUCCESS;
}

static inline OS_Error_t
decompressLoop(
    const uint8_t* ip,
    const uint8_t* iBoundary,
    uint8_t* op,
    const uint8_t* oBoundary)
{
    const uint8_t*  obuf = op;
    const size_t    olen = oBoundary - op;

    while (ip < iBoundary)
    {
        size_t slen;
        // Read number of occurences of symbol and the symbol itself
        ip = lenDeserializeOpt(ip, &slen);
        // Make sure we don't exceed the expected length
        if (slen + (op - obuf) > olen)
        {
            return OS_ERROR_ABORTED;
        }

        uint8_t sym = *(ip++);
        // Write symbol as often as we found it
        while (slen-- > 0 && op < oBoundary)
        {
            *(op++) = sym;
        }
    }
    return OS_SUCCESS;
}

static inline bool
isCompressParametersOk(
    const uint8_t* ibuf,
    size_t*        olen,
    uint8_t**      obuf)
{
    if (NULL == ibuf || NULL == olen || NULL == obuf)
    {
        return false;
    }
    return true;
}

static inline bool
isDecompressParametersOk(
    const size_t   ilen,
    const uint8_t* ibuf,
    size_t*        olen,
    uint8_t**      obuf)
{
    if (NULL == ibuf || NULL == olen || NULL == obuf ||
        ilen > RLECOMPRESSOR_MAX_INPUT_SIZE)
    {
        return false;
    }
    // Check magic header
    static const char* headString = "RLE";

    if (ilen >= strlen(headString) &&
        memcmp(ibuf, headString, strlen(headString)))
    {
        return false;
    }
    return true;
}

// Public functions ------------------------------------------------------------

OS_Error_t
RleCompressor_compress(
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    size_t*        olen,
    uint8_t**      obuf)
{
    uint8_t* op;
    size_t sz;

    if (isCompressParametersOk(ibuf, olen, obuf))
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (osz)
    {
        // If we get a buffer, make sure we can at least write the header to it
        sz = osz;
        if (osz < HEADER_LENGTH)
        {
            return OS_ERROR_BUFFER_TOO_SMALL;
        }
    }
    else
    {
        // If we did'nt get a buffer we allocate one with maximum possible size,
        // i.e., every symbol is encoded into two bytes + the header. We will
        // shrink the buffer later once the final size is known
        sz = (2 * ilen) + HEADER_LENGTH;
        if ((*obuf = malloc(sz)) == NULL)
        {
            return OS_ERROR_INSUFFICIENT_SPACE;
        }
    }

    op = *obuf;

    // Write some magic bytes as header
    *(op++) = 'R';
    *(op++) = 'L';
    *(op++) = 'E';

    // Write the uncompressed length
    BitConverter_putUint32LE(ilen, op);
    op += sizeof(uint32_t);

    const uint8_t* ip           = ibuf;
    const uint8_t* iBoundary    = ibuf + ilen;
    const uint8_t* oBoundary    = *obuf + sz;
    OS_Error_t err              = compressLoop(ip, iBoundary, op, oBoundary);
    if (OS_SUCCESS != err)
    {
        if (!osz)
        {
            free(*obuf);
        }
        return err;
    }

    *olen = op - (*obuf);

    // If we allocated a buffer, shrink it to the final size
    if (!osz)
    {
        *obuf = realloc(*obuf, *olen);
    }

    return OS_SUCCESS;
}

OS_Error_t
RleCompressor_decompress(
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    size_t*        olen,
    uint8_t**      obuf)
{
    if (isDecompressParametersOk(ilen, ibuf, olen, obuf))
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (ilen < HEADER_LENGTH)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    // Get the expected length after decompression
    size_t      my_olen = BitConverter_getUint32LE(ibuf);
    uint8_t*    my_obuf = NULL;

    const uint8_t* ip = ibuf + sizeof(uint32_t);

    // If we have given an output size, check it would fit; if the output
    // size is set to 0, that means we should allocate a buffer with the
    // expected size
    if (osz)
    {
        if (my_olen > osz)
        {
            return OS_ERROR_BUFFER_TOO_SMALL;
        }
    }
    else if ((my_obuf = malloc(my_olen)) == NULL)
    {
        return OS_ERROR_INSUFFICIENT_SPACE;
    }
    else
    {
        *obuf   = my_obuf;
        *olen   = my_olen;
    }

    const uint8_t* iBoundary    = ibuf + ilen;
    const uint8_t* oBoundary    = *obuf + *olen;
    OS_Error_t err              = decompressLoop(ip,
                                                 iBoundary,
                                                 *obuf,
                                                 oBoundary);
    if (err != OS_SUCCESS && !osz)
    {
        free(my_obuf);
        *olen = 0;
    }
    return err;
}

///@}
