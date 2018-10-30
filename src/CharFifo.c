/**
 * @addtogroup Commons
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

#include "lib_util/CharFifo.h"


/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static inline void
char_dtor(char* c)
{
    *c = -1;
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
    bool ok = false, retval = false;

    if (CharFifo_isFull(self))
    {
        CharFifo_pop(self);
        retval = true;
    }
    else { /* do nothing */ }

    ok = CharFifo_push(self, c);
    Debug_ASSERT(ok);

    return retval;
}

char
CharFifo_getAndPop(CharFifo* self)
{
    char c = * CharFifo_getFirst(self);
    bool ok = CharFifo_pop(self);
    Debug_ASSERT(ok);

    return c;
}

/* Private Functions -------------------------------------------------------- */

