/**
 * @addtogroup LibUtil
 * @{
 *
 * @file BitConverter.h
 *
 * @brief Converts base data types to an array of bytes, and an array of bytes
 * to base data types.
 *
 * Copyright (C) 2020, HENSOLDT Cyber GmbH
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define BitConverter_IS_BIG_ENDIAN() (*(uint16_t *)"\0\xff" < 0x100)

/* Exported functions ------------------------------------------------------- */
/**
 * @brief puts the bytes of a uint64_t argument \b arg in \b mem in a Big
 * Endian order
 */
void
BitConverter_putUint64BE(
    uint64_t arg,
    void* mem
);
/**
 * @brief puts the bytes of a uint64_t argument \b arg in \b mem in a Little
 * Endian order
 */
void
BitConverter_putUint64LE(
    uint64_t arg,
    void* mem
);
/**
 * @brief gets the uint64_t from the bytes placed in Big Endian order at the
 * memory location \b mem
 */
uint64_t
BitConverter_getUint64BE(const void* mem);
/**
 * @brief gets the uint64_t from the bytes placed in Little Endian order at the
 * memory location \b mem
 */
uint64_t
BitConverter_getUint64LE(const void* mem);
/**
 * @brief puts the bytes of a uint32_t argument \b arg in \b mem in a Big
 * Endian order
 */
void
BitConverter_putUint32BE(
    uint32_t arg,
    void* mem
);
/**
 * @brief puts the bytes of a uint32_t argument \b arg in \b mem in a Little
 * Endian order
 */
void
BitConverter_putUint32LE(
    uint32_t arg,
    void* mem
);
/**
 * @brief gets the uint32_t from the bytes placed in Big Endian order at the
 * memory location \b mem
 */
uint32_t
BitConverter_getUint32BE(const void* mem);
/**
 * @brief gets the uint32_t from the bytes placed in Little Endian order at the
 * memory location \b mem
 */
uint32_t
BitConverter_getUint32LE(const void* mem);
/**
 * @brief puts the bytes of a uint16_t argument \b arg in \b mem in a Big
 * Endian order
 */
void
BitConverter_putUint16BE(
    uint16_t arg,
    void* mem
);
/**
 * @brief puts the bytes of a uint16_t argument \b arg in \b mem in a Little
 * Endian order
 */
void
BitConverter_putUint16LE(
    uint16_t arg,
    void* mem
);
/**
 * @brief gets the uint16_t from the bytes placed in Big Endian order at the
 * memory location \b mem
 */
uint16_t
BitConverter_getUint16BE(const void* mem);
/**
 * @brief gets the uint16_t from the bytes placed in Little Endian order at the
 * memory location \b mem
 */
uint16_t
BitConverter_getUint16LE(const void* mem);



// static inline Functions ------------------------------------------------------------

/**
 * @brief puts the bytes of a uint64_t argument \b arg in \b mem with the
 * endianess order of the machine
 */
static inline void
BitConverter_putUint64(
    uint64_t arg,
    void* mem
)
{
    if (BitConverter_IS_BIG_ENDIAN())
    {
        BitConverter_putUint64BE(arg, mem);
    }
    else
    {
        BitConverter_putUint64LE(arg, mem);
    }
}
/**
 * @brief puts the bytes of a int64_t argument \b arg in \b mem in a Big
 * Endian order
 */
static inline void
BitConverter_putInt64BE(
    int64_t arg,
    void* mem
)
{
    BitConverter_putUint64BE((uint64_t) arg, mem);
}
/**
 * @brief puts the bytes of a int64_t argument \b arg in \b mem in a Little
 * Endian order
 */
static inline void
BitConverter_putInt64LE(
    int64_t arg,
    void* mem
)
{
    BitConverter_putUint64LE((uint64_t) arg, mem);
}
/**
 * @brief puts the bytes of a int64_t argument \b arg in \b mem with the
 * endianess order of the machine
 */
static inline void
BitConverter_putInt64(
    int64_t arg,
    void* mem
)
{
    BitConverter_putUint64((uint64_t) arg, mem);
}
/**
 * @brief gets the uint64_t from the bytes placed in the endianess order of the
 * machine at the memory location \b mem
 */
static inline uint64_t
BitConverter_getUint64(const void* mem)
{
    if (BitConverter_IS_BIG_ENDIAN())
    {
        return BitConverter_getUint64BE(mem);
    }
    else
    {
        return BitConverter_getUint64LE(mem);
    }
}
/**
 * @brief gets the int64_t from the bytes placed in the endianess order of the
 * machine at the memory location \b mem
 */
static inline int64_t
BitConverter_getInt64(const void* mem)
{
    return (int64_t) BitConverter_getUint64(mem);
}
/**
 * @brief puts the bytes of a uint32_t argument \b arg in \b mem with the
 * endianess order of the machine
 */
static inline void
BitConverter_putUint32(
    uint32_t arg,
    void* mem
)
{
    if (BitConverter_IS_BIG_ENDIAN())
    {
        BitConverter_putUint32BE(arg, mem);
    }
    else
    {
        BitConverter_putUint32LE(arg, mem);
    }
}
/**
 * @brief puts the bytes of a int32_t argument \b arg in \b mem in a Big
 * Endian order
 */
static inline void
BitConverter_putInt32BE(
    int32_t arg,
    void* mem
)
{
    BitConverter_putUint32BE((uint32_t) arg, mem);
}
/**
 * @brief puts the bytes of a int32_t argument \b arg in \b mem in a Little
 * Endian order
 */
static inline void
BitConverter_putInt32LE(
    int32_t arg,
    void* mem
)
{
    BitConverter_putUint32LE((uint32_t) arg, mem);
}
/**
 * @brief puts the bytes of a int32_t argument \b arg in \b mem with the
 * endianess order of the machine
 */
static inline void
BitConverter_putInt32(
    int32_t arg,
    void* mem
)
{
    BitConverter_putUint32((uint32_t) arg, mem);
}
/**
 * @brief gets the uint32_t from the bytes placed in the endianess order of the
 * machine at the memory location \b mem
 */
static inline uint32_t
BitConverter_getUint32(const void* mem)
{
    if (BitConverter_IS_BIG_ENDIAN())
    {
        return BitConverter_getUint32BE(mem);
    }
    else
    {
        return BitConverter_getUint32LE(mem);
    }
}
/**
 * @brief gets the int32_t from the bytes placed in the endianess order of the
 * machine at the memory location \b mem
 */
static inline int32_t
BitConverter_getInt32(const void* mem)
{
    return (int32_t) BitConverter_getUint32(mem);
}
/**
 * @brief puts the bytes of a uint16_t argument \b arg in \b mem with the
 * endianess order of the machine
 */
static inline void
BitConverter_putUint16(
    uint16_t arg,
    void* mem
)
{
    if (BitConverter_IS_BIG_ENDIAN())
    {
        BitConverter_putUint16BE(arg, mem);
    }
    else
    {
        BitConverter_putUint16LE(arg, mem);
    }
}
/**
 * @brief puts the bytes of a int16_t argument \b arg in \b mem in a Big
 * Endian order
 */
static inline void
BitConverter_putInt16BE(
    int16_t arg,
    void* mem
)
{
    BitConverter_putUint16BE((uint16_t) arg, mem);
}
/**
 * @brief puts the bytes of a int16_t argument \b arg in \b mem in a Little
 * Endian order
 */
static inline void
BitConverter_putInt16LE(
    int16_t arg,
    void* mem
)
{
    BitConverter_putUint16LE((uint16_t) arg, mem);
}
/**
 * @brief puts the bytes of a int16_t argument \b arg in \b mem with the
 * endianess order of the machine
 */
static inline void
BitConverter_putInt16(
    int16_t arg,
    void* mem
)
{
    BitConverter_putUint16((uint16_t) arg, mem);
}
/**
 * @brief gets the uint16_t from the bytes placed in the endianess order of the
 * machine at the memory location \b mem
 */
static inline uint16_t
BitConverter_getUint16(const void* mem)
{
    if (BitConverter_IS_BIG_ENDIAN())
    {
        return BitConverter_getUint16BE(mem);
    }
    else
    {
        return BitConverter_getUint16LE(mem);
    }
}
/**
 * @brief gets the int16_t from the bytes placed in the endianess order of the
 * machine at the memory location \b mem
 */
static inline int16_t
BitConverter_getInt16(const void* mem)
{
    return (int16_t) BitConverter_getUint16(mem);
}

///@}
