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
 * @file CharFifo.h
 *
 * @brief Fifo class of chars based on FifoT.h template
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#if !defined(CHAR_FIFO_H)
#define CHAR_FIFO_H


/* Includes ------------------------------------------------------------------*/

#include "lib_utils/FifoT.h"


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

///@}
