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

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

// The worst case length for an encoded symbol is 4 bytes for the length indicator
// and one byte for the symbol itself
#define ENCODED_SYMBOL_MAX_SIZE (sizeof(uint32_t) + 1)
// Length of header we always have, i.e., "RLE" and four lenght bytes indicating
// decompressed length
#define HEADER_LENGTH (sizeof(uint32_t) + 3)

// Private functions -----------------------------------------------------------

static size_t
lenNeedsBytes(
    const size_t len)
{
    /*
     * How many bytes will we need to serialize this length value?
     */

    return (len < (1u << 6))  ? 1 :
           (len < (1u << 14)) ? 2 :
           (len < (1u << 22)) ? 3 : 4;
}

static uint8_t*
lenSerializeOpt(
    const size_t sz,
    const size_t len,
    uint8_t*     p)
{
    /*
     * Sligtly optimized serialization routine: We do not need to always use a
     * full 4 bytes to encode the length, if the length does not require that.
     * The first 2 bits of the length field always indicate how many bytes were
     * used in the encoding.
     */

    switch (sz)
    {
    case 1:
        *(p++) = 0b00000000 |  (len & 0x3F);
        break;
    case 2:
        *(p++) = 0b01000000 | ((len >>  8) & 0x3F);
        *(p++) =               (len >>  0) & 0xFF;
        break;
    case 3:
        *(p++) = 0b10000000 | ((len >> 16) & 0x3F);
        *(p++) =               (len >>  8) & 0xFF;
        *(p++) =               (len >>  0) & 0xFF;
        break;
    default:
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
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    size_t*        olen,
    uint8_t*       obuf)
{
    const uint8_t* ip = ibuf;
    uint8_t sym, *op = obuf;
    size_t slen, sz;

    while (ip < (ibuf + ilen))
    {
        sym  = *ip;
        slen = 1;
        // Check how often the symbol is found in sequence
        while (++ip < (ibuf + ilen) && *ip == sym)
        {
            slen++;
        }
        // Make sure we can fit the serialized len bytes and the symbol
        sz = lenNeedsBytes(slen);
        if ((op + sz + 1) < (obuf + osz))
        {
            op = lenSerializeOpt(sz, slen, op);
            *(op++) = sym;
        }
        else
        {
            return OS_ERROR_ABORTED;
        }
    }

    *olen = op - obuf;

    return OS_SUCCESS;
}

static inline OS_Error_t
decompressLoop(
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    uint8_t*       obuf)
{
    const uint8_t* ip = ibuf;
    uint8_t sym, *op = obuf;
    size_t slen;

    while (ip < (ibuf + ilen))
    {
        // Read number of occurences of symbol and the symbol itself
        ip  = lenDeserializeOpt(ip, &slen);
        sym = *(ip++);
        // Make sure we don't exceed the expected length
        if (slen + (op - obuf) > osz)
        {
            return OS_ERROR_ABORTED;
        }
        // Write symbol as often as we found it
        while (slen-- > 0)
        {
            *(op++) = sym;
        }
    }

    return OS_SUCCESS;
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
    OS_Error_t err;
    size_t sz, my_olen;
    uint8_t* my_obuf;

    if (NULL == obuf || NULL == ibuf || NULL == olen)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
    if (ilen > RLECOMPRESSOR_MAX_INPUT_SIZE)
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
        my_obuf = *obuf;
    }
    else
    {
        // If we didn't get a buffer we allocate one with maximum possible size,
        // i.e., every symbol is encoded into two bytes + the header. We will
        // shrink the buffer later once the final size is known
        sz = (2 * ilen) + HEADER_LENGTH;
        if ((my_obuf = malloc(sz)) == NULL)
        {
            return OS_ERROR_INSUFFICIENT_SPACE;
        }
    }

    // Write some magic bytes as header
    memcpy(my_obuf, "RLE", 3);
    // Write the uncompressed length
    BitConverter_putUint32LE(ilen, my_obuf + 3);

    if ((err = compressLoop(ilen, ibuf, sz - HEADER_LENGTH, &my_olen,
                            my_obuf + HEADER_LENGTH)) != OS_SUCCESS)
    {
        *olen = 0;
        if (!osz)
        {
            free(my_obuf);
            *obuf = NULL;
        }
    }
    else
    {
        *olen = my_olen + HEADER_LENGTH;
        if (!osz)
        {
            // If we allocated a buffer, shrink it to the final size
            *obuf  = realloc(my_obuf, *olen);
        }
    }

    return err;
}

OS_Error_t
RleCompressor_decompress(
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    size_t*        olen,
    uint8_t**      obuf)
{
    OS_Error_t err;
    size_t my_olen;
    uint8_t* my_obuf;

    if (NULL == ibuf || NULL == olen || NULL == obuf)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
    if (ilen < HEADER_LENGTH)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    // Check magic header
    if (memcmp(ibuf, "RLE", 3))
    {
        return OS_ERROR_INVALID_STATE;
    }
    // Get the expected length after decompression
    my_olen = BitConverter_getUint32LE(ibuf + 3);

    // If we have given an output size, check it would fit; if the output
    // size is set to 0, that means we should allocate a buffer with the
    // expected size
    if (osz)
    {
        if (my_olen > osz)
        {
            return OS_ERROR_BUFFER_TOO_SMALL;
        }
        my_obuf = *obuf;
    }
    else if ((my_obuf = malloc(my_olen)) == NULL)
    {
        return OS_ERROR_INSUFFICIENT_SPACE;
    }

    if ((err = decompressLoop(ilen - HEADER_LENGTH, ibuf + HEADER_LENGTH, my_olen,
                              my_obuf)) != OS_SUCCESS)
    {
        *olen = 0;
        if (!osz)
        {
            free(my_obuf);
            *obuf = NULL;
        }
    }
    else
    {
        *olen = my_olen;
        *obuf = my_obuf;
    }

    return err;
}

///@}