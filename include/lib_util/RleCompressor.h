/**
 * @addtogroup LibUtil
 * @{
 *
 * @file RleCompressor.h
 *
 * @brief Compress a buffer with a simple Run Length Encoder
 *
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 */

#pragma once

#include "OS_Error.h"

#include <stdint.h>
#include <stddef.h>

/**
 * We can handle at most (2^30)-1 bits, because that is what we can encode for a
 * single symbol (symbol lengths fields are between 8-32 bit, but we reserve 2
 * bits per length field to indicate the length of the length field itself)
 */
#define RLECOMPRESSOR_MAX_INPUT_SIZE ((1ul << 30) - 1)

/**
 * @brief Compress a buffer with RLE encoder
 *
 * Performs RLE encoding on \p ibuf, can allocate correctly sized output buffer
 * \p obuf if \p osz is set to zero.
 *
 * @param ilen (required) length of input buffer data
 * @param ibuf (required) input buffer
 * @param osz (optional) size of output buffer, set to 0 to let function allocate
 *  correctly sized \p obuf
 * @param olen (required) lenght of compressed data in \p obuf
 * @param obuf (required) pointer to output buffer
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_BUFFER_TOO_SMALL if any of the buffers was too small
 * @retval OS_ERROR_INSUFFICIENT_SPACE if some allocation failed
 * @retval OS_ERROR_ABORTED if encoding was aborted due to reaching limit
 *  of \p obuf buffer
 */
OS_Error_t
RleCompressor_compress(
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    size_t*        olen,
    uint8_t**      obuf);

/**
 * @brief Decompress a buffer with RLE encoder
 *
 * Performs RLE decoding on \p ibuf, can allocate correctly sized output buffer
 * \p obuf if \p osz is set to zero.
 *
 * @param ilen (required) length of input buffer data
 * @param ibuf (required) input buffer with compressed data
 * @param osz (optional) size of output buffer, set to 0 to let function allocate
 *  correctly sized \p obuf
 * @param olen (required) lenght of compressed data in \p obuf
 * @param obuf (required) pointer to output buffer
 *
 * @return an error code
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_BUFFER_TOO_SMALL if any of the buffers was too small
 * @retval OS_ERROR_INSUFFICIENT_SPACE if some allocation failed
 * @retval OS_ERROR_ABORTED if encoding was aborted due to reaching limit
 *  of \p obuf buffer
 */
OS_Error_t
RleCompressor_decompress(
    const size_t   ilen,
    const uint8_t* ibuf,
    const size_t   osz,
    size_t*        olen,
    uint8_t**      obuf);

///@}