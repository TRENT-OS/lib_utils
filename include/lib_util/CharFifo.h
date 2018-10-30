/**
 * @addtogroup Commons
 * @{
 *
 * @file CharFifo.h
 *
 *
 *
 * @author fw team
 * @version 1.0
 * @date 01/10/2010
 *
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#if !defined(CHAR_FIFO_H)
#define CHAR_FIFO_H


/* Includes ------------------------------------------------------------------*/

#include "lib_util/FifoT.h"


/* Exported types ------------------------------------------------------------*/

typedef
FifoT_TYPE(char, size_t)
CharFifo;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

FifoT_DECLARE(char, CharFifo, size_t);

char
CharFifo_getAndPop(CharFifo* self);

bool
CharFifo_forcedPush(CharFifo* self, char const* c);

#endif /* <HEADER_UNIQUE_SYMBOL_H> */

