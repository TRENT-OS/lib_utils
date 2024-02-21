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
 * @file CharFifo.c
 *
 * @author fw team
 * @version 1.0
 * @date 01/10/2010
 *
 * @notes
 * @history
 */

/* Includes ------------------------------------------------------------------*/

#include "lib_compiler/compiler.h"
#include "lib_utils/CharFifo.h"


/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static inline void
char_dtor(char* c)
{
    return;
}

static inline bool
char_ctorCopy(char* target, char const* source )
{
    Debug_ASSERT(target != NULL && source != NULL);
    *target = *source;
    return true;
}


/* Public functions ----------------------------------------------------------*/

FifoT_DEFINE(char, CharFifo, size_t)

bool
CharFifo_forcedPush(CharFifo* self, char const* c)
{
    bool retval = false;

    if (CharFifo_isFull(self))
    {
        CharFifo_pop(self);
        retval = true;
    }
    else { /* do nothing */ }

    DECL_UNUSED_VAR(const bool ok) = CharFifo_push(self, c);
    Debug_ASSERT(ok);

    return retval;
}

char
CharFifo_getAndPop(CharFifo* self)
{
    char c = * CharFifo_getFirst(self);
    DECL_UNUSED_VAR(const bool ok) = CharFifo_pop(self);
    Debug_ASSERT(ok);

    return c;
}

/* Private Functions -------------------------------------------------------- */

///@}
