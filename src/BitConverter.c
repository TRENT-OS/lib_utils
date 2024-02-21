/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @addtogroup lib_utils
 * @{
 *
 * @file BitConverter.c
 */

/* Includes ------------------------------------------------------------------*/
#include "lib_utils/BitConverter.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void
BitConverter_putUint64BE(uint64_t arg, void* mem)
{
    char* bytes = (char*) mem;

    bytes[0] = (arg & 0xff00000000000000) >> 56;
    bytes[1] = (arg & 0x00ff000000000000) >> 48;
    bytes[2] = (arg & 0x0000ff0000000000) >> 40;
    bytes[3] = (arg & 0x000000ff00000000) >> 32;
    bytes[4] = (arg & 0x00000000ff000000) >> 24;
    bytes[5] = (arg & 0x0000000000ff0000) >> 16;
    bytes[6] = (arg & 0x000000000000ff00) >> 8;
    bytes[7] = (arg & 0x00000000000000ff);
}

void
BitConverter_putUint64LE(uint64_t arg, void* mem)
{
    char* bytes = (char*) mem;

    bytes[7] = (arg & 0xff00000000000000) >> 56;
    bytes[6] = (arg & 0x00ff000000000000) >> 48;
    bytes[5] = (arg & 0x0000ff0000000000) >> 40;
    bytes[4] = (arg & 0x000000ff00000000) >> 32;
    bytes[3] = (arg & 0x00000000ff000000) >> 24;
    bytes[2] = (arg & 0x0000000000ff0000) >> 16;
    bytes[1] = (arg & 0x000000000000ff00) >> 8;
    bytes[0] = (arg & 0x00000000000000ff);
}

void
BitConverter_putUint32BE(uint32_t arg, void* mem)
{
    char* bytes = (char*) mem;

    bytes[0] = (arg & 0xff000000) >> 24;
    bytes[1] = (arg & 0x00ff0000) >> 16;
    bytes[2] = (arg & 0x0000ff00) >> 8;
    bytes[3] = (arg & 0x000000ff);
}

void
BitConverter_putUint32LE(uint32_t arg, void* mem)
{
    char* bytes = (char*) mem;

    bytes[3] = (arg & 0xff000000) >> 24;
    bytes[2] = (arg & 0x00ff0000) >> 16;
    bytes[1] = (arg & 0x0000ff00) >> 8;
    bytes[0] = (arg & 0x000000ff);
}

void
BitConverter_putUint16BE(uint16_t arg, void* mem)
{
    char* bytes = (char*) mem;

    bytes[0] = (arg & 0xff00) >> 8;
    bytes[1] = (arg & 0x00ff);
}

void
BitConverter_putUint16LE(uint16_t arg, void* mem)
{
    char* bytes = (char*) mem;

    bytes[1] = (arg & 0xff00) >> 8;
    bytes[0] = (arg & 0x00ff);
}

uint64_t
BitConverter_getUint64BE(const void* mem)
{
    char* bytes = (char*) mem;
    uint64_t retval = 0;

    retval |= ((uint64_t) bytes[0]) << 56;
    retval |= ((uint64_t) bytes[1]) << 48;
    retval |= ((uint64_t) bytes[2]) << 40;
    retval |= ((uint64_t) bytes[3]) << 32;
    retval |= ((uint64_t) bytes[4]) << 24;
    retval |= ((uint64_t) bytes[5]) << 16;
    retval |= ((uint64_t) bytes[6]) << 8;
    retval |= ((uint64_t) bytes[7]);

    return retval;
}

uint64_t
BitConverter_getUint64LE(const void* mem)
{
    char* bytes = (char*) mem;
    uint64_t retval = 0;

    retval |= ((uint64_t) bytes[7]) << 56;
    retval |= ((uint64_t) bytes[6]) << 48;
    retval |= ((uint64_t) bytes[5]) << 40;
    retval |= ((uint64_t) bytes[4]) << 32;
    retval |= ((uint64_t) bytes[3]) << 24;
    retval |= ((uint64_t) bytes[2]) << 16;
    retval |= ((uint64_t) bytes[1]) << 8;
    retval |= ((uint64_t) bytes[0]);

    return retval;
}

uint32_t
BitConverter_getUint32BE(const void* mem)
{
    char* bytes = (char*) mem;
    uint32_t retval = 0;

    retval |= ((uint32_t) bytes[0]) << 24;
    retval |= ((uint32_t) bytes[1]) << 16;
    retval |= ((uint32_t) bytes[2]) << 8;
    retval |= ((uint32_t) bytes[3]);

    return retval;
}

uint32_t
BitConverter_getUint32LE(const void* mem)
{
    char* bytes = (char*) mem;
    uint32_t retval = 0;

    retval |= ((uint32_t) bytes[3]) << 24;
    retval |= ((uint32_t) bytes[2]) << 16;
    retval |= ((uint32_t) bytes[1]) << 8;
    retval |= ((uint32_t) bytes[0]);

    return retval;
}

uint16_t
BitConverter_getUint16BE(const void* mem)
{
    char* bytes = (char*) mem;
    uint16_t retval = 0;

    retval |= ((uint16_t) bytes[0]) << 8;
    retval |= ((uint16_t) bytes[1]);

    return retval;
}

uint16_t
BitConverter_getUint16LE(const void* mem)
{
    char* bytes = (char*) mem;
    uint16_t retval = 0;

    retval |= ((uint16_t) bytes[1]) << 8;
    retval |= ((uint16_t) bytes[0]);

    return retval;
}

/* Private Functions -------------------------------------------------------- */
///@}

