/**
 * @addtogroup LibUtil
 * @{
 *
 * @file PointerVector.h
 *
 */
#if !defined(POINTER_VECTOR_H)
#define POINTER_VECTOR_H


#include "lib_util/VectorT.h"

typedef void* Pointer;

void Pointer_dtor(Pointer* el);
bool Pointer_ctorCopy(Pointer* dst, Pointer const* src);
bool Pointer_ctorMove(Pointer* dst, Pointer const* src);
bool Pointer_assign(Pointer* dst, Pointer const* src);

VectorT_DECLARE(Pointer, PointerVector, size_t);

#endif /* <HEADER_UNIQUE_SYMBOL_H> */

