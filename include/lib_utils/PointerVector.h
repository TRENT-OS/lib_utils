/**
 * @addtogroup lib_utils
 * @{
 *
 * @file PointerVector.h
 *
 * @brief Vector class of void* pointers based on VectorT.h template
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */
#pragma once

#include "lib_utils/VectorT.h"

#include <stdint.h>

typedef intptr_t Pointer;

void Pointer_dtor(Pointer* el);
bool Pointer_ctorCopy(Pointer* dst, Pointer const* src);
bool Pointer_ctorMove(Pointer* dst, Pointer const* src);
bool Pointer_assign(Pointer* dst, Pointer const* src);
bool Pointer_isEqual(Pointer const * a, Pointer const * b);

VectorT_DECLARE(Pointer, PointerVector, size_t);

#define PointerVector_SIZE_OF_BUFFER(numItems)\
    VectorT_SIZE_OF_BUFFER(Pointer, numItems)

///@}
