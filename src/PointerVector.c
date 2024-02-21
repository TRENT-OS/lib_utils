/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "lib_utils/PointerVector.h"

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

bool
Pointer_isEqual(Pointer const* a, Pointer const* b)
{
    return (*a == *b);
}
