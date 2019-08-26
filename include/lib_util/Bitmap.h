/**
 * @addtogroup LibUtil
 * @{
 *
 * @file Bitmap.h
 *
 * @brief Simple bitmap management : Bit Index start from 0.
 *
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

typedef uint64_t volatile  BitMap64;
typedef uint32_t volatile  BitMap32;
typedef uint16_t volatile  BitMap16;
typedef uint8_t  volatile  BitMap8;
typedef int      volatile  BitMapInt;

#define BitMap_ALL_SET_MASK             0xFFFFFFFFFFFFFFFF  //8 byte FF

#define BitMap_MASK_OF_BIT(b)           (1 << (b))

#define BitMap_GET_BIT(arg, bitNum)     ((arg) & BitMap_MASK_OF_BIT(bitNum))
#define BitMap_SET_BIT(arg, bitNum)     ((arg) |=  BitMap_MASK_OF_BIT(bitNum))
#define BitMap_CLR_BIT(arg, bitNum)     ((arg) &= ~BitMap_MASK_OF_BIT(bitNum))
#define BitMap_FLIP_BIT(arg, bitNum)    ((arg) ^=  BitMap_MASK_OF_BIT(bitNum))

#define BitMap_GET_MASK(arg, mask)      ((arg) & (mask))
#define BitMap_SET_MASK(arg, mask)      ((arg) |= (mask))
#define BitMap_CLR_MASK(arg, mask)      ((arg) &= ~(mask))
#define BitMap_FLIP_MASK(arg, mask)     ((arg) ^= (mask))
#define BitMap_DIFF_MASK(arg, mask)     ((arg) ^ (mask))

#define BitMap_IS_EMPTY(arg)            ((arg) == 0)
#define BitMap_CHK_ANY_FLAG(arg, mask)  (((arg) & (mask)) != 0)
#define BitMap_CHK_ALL_FLAGS(arg, mask) (((arg) & (mask)) == (mask))
#define BitMap_DOWN_SHIFT(arg, shift)   ((arg) >> (shift))
#define BitMap_UP_SHIFT(arg, shift)     ((arg) << (shift))
#define BitMap_SET_ALL(arg)             ((arg) |= (BitMap_ALL_SET_MASK))
#define BitMap_CLEAR_ALL(arg)           ((arg) &= (~BitMap_ALL_SET_MASK))


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* BITMAP_H */

///@}
