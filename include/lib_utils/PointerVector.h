/**
 * @addtogroup lib_utils
 * @{
 *
 * @file PointerVector.h
 *
 * @brief Vector class of void* pointers based on VectorT.h template
 *
 * Copyright (C) 2019, HENSOLDT Cyber GmbH
 */
#pragma once

#include "lib_utils/VectorT.h"

typedef void* Pointer;

void Pointer_dtor(Pointer* el);
bool Pointer_ctorCopy(Pointer* dst, Pointer const* src);
bool Pointer_ctorMove(Pointer* dst, Pointer const* src);
bool Pointer_assign(Pointer* dst, Pointer const* src);

VectorT_DECLARE(Pointer, PointerVector, size_t);

#define PointerVector_GET_SIZE_BY_CAPACITY(capacity)\
    VectorT_GET_SIZE_BY_CAPACITY(Pointer, capacity)

///@}
