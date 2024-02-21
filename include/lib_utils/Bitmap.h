/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @addtogroup lib_utils
 * @{
 *
 * @file Bitmap.h
 *
 * @brief Simple bitmap management : Bit Index start from 0.
 */

#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

typedef uint64_t volatile  Bitmap64;
typedef uint32_t volatile  Bitmap32;
typedef uint16_t volatile  Bitmap16;
typedef uint8_t  volatile  Bitmap8;
typedef int      volatile  BitmapInt;

#define Bitmap_ALL_SET_MASK             0xFFFFFFFFFFFFFFFF  //8 byte FF

#define Bitmap_MASK_OF_BIT(b)           (1 << (b))

#define Bitmap_GET_BIT(arg, bitNum)     ((arg) & Bitmap_MASK_OF_BIT(bitNum))
#define Bitmap_SET_BIT(arg, bitNum)     ((arg) |=  Bitmap_MASK_OF_BIT(bitNum))
#define Bitmap_CLR_BIT(arg, bitNum)     ((arg) &= ~Bitmap_MASK_OF_BIT(bitNum))
#define Bitmap_FLIP_BIT(arg, bitNum)    ((arg) ^=  Bitmap_MASK_OF_BIT(bitNum))

#define Bitmap_GET_MASK(arg, mask)      ((arg) & (mask))
#define Bitmap_SET_MASK(arg, mask)      ((arg) |= (mask))
#define Bitmap_CLR_MASK(arg, mask)      ((arg) &= ~(mask))
#define Bitmap_FLIP_MASK(arg, mask)     ((arg) ^= (mask))
#define Bitmap_DIFF_MASK(arg, mask)     ((arg) ^ (mask))

#define Bitmap_IS_EMPTY(arg)            ((arg) == 0)
#define Bitmap_CHK_ANY_FLAG(arg, mask)  (((arg) & (mask)) != 0)
#define Bitmap_CHK_ALL_FLAGS(arg, mask) (((arg) & (mask)) == (mask))
#define Bitmap_DOWN_SHIFT(arg, shift)   ((arg) >> (shift))
#define Bitmap_UP_SHIFT(arg, shift)     ((arg) << (shift))
#define Bitmap_SET_ALL(arg)             ((arg) |= (Bitmap_ALL_SET_MASK))
#define Bitmap_CLEAR_ALL(arg)           ((arg) &= (~Bitmap_ALL_SET_MASK))


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* BITMAP_H */

///@}
