/*  Copyright (C) 2020, Hensoldt Cyber GmbH */

#include "lib_util/PointerVector.h"

VectorT_DEFINE(Pointer, PointerVector, size_t);

void
Pointer_dtor(Pointer* el)
{
    return;
}

bool
Pointer_assign(Pointer* dst, Pointer const* src)
{
    *dst = *src;
    return true;
}

bool
Pointer_ctorCopy(Pointer* dst, Pointer const* src)
{
    return Pointer_assign(dst, src);
}

bool
Pointer_ctorMove(Pointer* dst, Pointer const* src)
{
    return Pointer_assign(dst, src);
}
