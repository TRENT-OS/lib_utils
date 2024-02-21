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
 * @file FifoT.h
 *
 * @brief Fifo container template
 */
#if !defined(FIFOT_H)
#define FIFOT_H

#include "lib_compiler/compiler.h"
#include "lib_debug/Debug.h"

#include <stdbool.h>


/**
 * FIFO container template.
 * The FifoT is a template that can be used to define a container
 * implementing a FIFO strategy with any kind of type.
 * If the name of the containee is T, then the following operations are
 * required:
 *
 * bool T_ctorCopy(T* target, T const* source);
 * void T_dtor(T* object);
 *
 * The first function must implement a copy constructor, i.e. should construct
 * a copy of the object pointed to by ''source'' into the memory pointed to by
 * ''target''. It is required to return true if the construction has been
 * successful or false if the object couldn't have been built.
 * The latter is the object destructor and has the responsibility to free the
 * resources associated to the object (but not the memory used by the object
 * instance).
 */

#define FifoT_TYPE(T__, SIZE_T__)                                           \
struct {                                                                    \
    T__* fifo;                                                              \
    volatile SIZE_T__ first;                                                \
    volatile SIZE_T__ last;                                                 \
    volatile SIZE_T__ in;                                                   \
    volatile SIZE_T__ out;                                                  \
    volatile SIZE_T__ capacity;                                             \
}

/**
 * FifoT class constructor.
 *
 * The constructor initializes an empty fifo of the given size.
 *
 * @param self a pointer to the container itself.
 * @param buffer a pointer to the memory buffer. Must be at the least
 * sizeof(T__) * capacity bytes big.
 * @param capacity the maximum number of elements that the fifo can hold in its
 * memory buffer.
 * @return false if the constructor is unable to reserve enough dynamic memory
 *         for the container.
 *
 * @memberof FifoT
 */

#define FifoT_CTOR_DECL(T__, N__, SIZE_T__)                                 \
    bool N__##_ctor(N__* self, void* buffer, SIZE_T__ capacity)

/**
 * Destructor. Releases all the resources allocated by the fifo. Referenced
 * strings are free'd too.
 *
 * @param self a pointer to the container itself.
 *
 * @memberof FifoT
 */

#define FifoT_DTOR_DECL(T__, N__)                                           \
    void N__##_dtor(N__* self)

/**
 * Checks for emptiness.
 *
 * This method returns true if the fifo is empty, false otherwise.
 *
 * @param self a pointer to the container itself.
 * @return true if the container is empty.
 *
 * @memberof FifoT
 */
#define FifoT_ISEMPTY_DECL(T__,N__)                                         \
     bool N__##_isEmpty(N__ const* self)

/**
 * Method to check for container fullness.
 *
 * @param self a pointer to the container itself.
 * @return true if the container is full, i.e. getSize() equals the container
 *         capacity as set in the constructor. false is returned otherwise.
 *
 * @memberof FifoT
 */
#define FifoT_ISFULL_DECL(T__,N__)                                          \
    bool N__##_isFull(N__ const* self)

/**
 * Retrieves the number of elements in the container.
 *
 * @param self a pointer to the container itself.
 * @return the number of elements.
 *
 * @memberof FifoT
 */
#define FifoT_GETSIZE_DECL(T__, N__, SIZE_T__)                              \
SIZE_T__ N__##_getSize(N__ const* self)

#define FifoT_GETCAPACITY_DECL(T__, N__, SIZE_T__)                          \
SIZE_T__ N__##_getCapacity(N__ const* self)

/**
 * Pushes an item into the fifo.
 * The item is copied into the fifo, therefore the caller should take care
 * of handling the life cycle of the object pointed to by the string parameter.
 *
 * @param self a pointer to the fifo itself.
 * @param item a pointer to the item to push.
 *
 * @return true if the item has been succesfully inserted into the queue,
 *         false if the queue is full and ''item'' cannot be inserted.
 *
 * @memberof FifoT
 */

#define FifoT_PUSH_DECL(T__, N__)                                           \
bool N__##_push(N__* self, T__ const* string)

/**
 * Removes the first element in the fifo. The string in the queue is destroyed.
 *
 * @param self a pointer to the container itself.
 *
 * @return true if the operation was succesful, false if this method is invoked
 *         on an empty container.
 *
 * @memberof FifoT
 */
#define FifoT_POP_DECL(T__, N__)                                            \
bool N__##_pop(N__* self)

/**
 * Retrieves the first element of the fifo, i.e. the one pushed first in the
 * container, i.e. the first is going to be extracted.
 *
 * @note that the returned value is just a reference to the top element. If
 *       that element is popped or the fifo object is destroyed then the
 *       reference returned by this function is no longer valid.
 *
 * @param self a pointer to the container itself.
 * @return a pointer to the first element of the fifo or NULL if the fifo is
 *         empty.
 *
 * @memberof FifoT
 */

#define FifoT_GETFIRST_DECL(T__, N__)                                       \
T__ const* N__##_getFirst(N__ const* self)

/**
 * Clears the fifo.
 *
 * This method erases all the fifo contents. The fifo is left empty.
 *
 * @param self a pointer to the container itself.
 *
 * @memberof FifoT
 */
#define FifoT_CLEAR_DECL(T__, N__)                                          \
void N__##_clear(N__* self)

/**
 * Apply a given function to all the elements in the container.
 *
 * @param self a pointer to the container itself.
 * @param fn the function to apply. This function is called once for each
 *            element, starting from the oldest, i.e. the one returned by
 *            #getFirst() method.
 * @param context a context pointer that is passed along with each element to
 *                 the #fn function.
 * @return the num of elem processed
 *
 * @note the applied function has the following signature:
 * @code
 *        bool fn(void* context, T__ const* element, SIZE_T__ pos)
 * @endcode
 *        Where context is the pointer supplied to the constApply method,
 *        element is the current element, pos is the current position and
 *        the return value has the "continue" semantic.
 *
 *
 * @memberof FifoT
 *
*/

#define FifoT_CONST_APPLY_DECL(T__, N__, SIZE_T__)                          \
typedef bool (*N__##_applyFn)(void* context,                                \
                                T__ const* element,                         \
                                SIZE_T__ pos);                              \
SIZE_T__                                                                    \
N__##_constApply(N__ const* self, N__##_applyFn fn, void* context)


#define FifoT_DECLARE(T__, N__, SIZE_T__)                                   \
         FifoT_CTOR_DECL(T__, N__, SIZE_T__);                               \
         FifoT_DTOR_DECL(T__, N__);                                         \
         FifoT_ISEMPTY_DECL(T__, N__);                                      \
         FifoT_ISFULL_DECL(T__, N__);                                       \
         FifoT_GETSIZE_DECL(T__, N__, SIZE_T__);                            \
         FifoT_GETCAPACITY_DECL(T__, N__, SIZE_T__);                        \
         FifoT_PUSH_DECL(T__, N__);                                         \
         FifoT_POP_DECL(T__, N__);                                          \
         FifoT_GETFIRST_DECL(T__, N__);                                     \
         FifoT_CLEAR_DECL(T__, N__);                                        \
         FifoT_CONST_APPLY_DECL(T__, N__, SIZE_T__)


// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::: Implementation :::
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define FifoT_CTOR_IMPL(T__,N__, SIZE_T__)                                  \
bool N__##_ctor(N__* self,                                                  \
                void*    buffer,                                            \
                SIZE_T__ capacity)                                          \
{                                                                           \
    Debug_ASSERT_SELF(self);                                                \
                                                                            \
    self->fifo = buffer;                                                    \
                                                                            \
    if (self->fifo == NULL)                                                 \
    {                                                                       \
        return false;                                                       \
    }                                                                       \
    self->first = 0;                                                        \
    self->last  = 0;                                                        \
    self->in    = 0;                                                        \
    self->out   = 0;                                                        \
    self->capacity = capacity;                                              \
    return true;                                                            \
}

#define FifoT_DTOR_IMPL(T__, N__)                                           \
void                                                                        \
N__##_dtor(N__* self)                                                       \
{                                                                           \
    N__##_clear(self);                                                      \
}

#define FifoT_ISEMPTY_IMPL(T__, N__)                                        \
bool                                                                        \
N__##_isEmpty(N__ const* self)                                              \
{                                                                           \
    return self->in == self->out;                                           \
}

#define FifoT_ISFULL_IMPL(T__, N__)                                         \
bool                                                                        \
N__##_isFull(N__ const* self)                                               \
{                                                                           \
    return N__##_getSize(self) == self->capacity;                           \
}

#define FifoT_GETSIZE_IMPL(T__, N__, SIZE_T__)                              \
SIZE_T__                                                                    \
N__##_getSize(N__ const* self)                                              \
{                                                                           \
    if (self->in >= self->out)                                              \
    {                                                                       \
        return self->in - self->out;                                        \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        return self->out - self->in;                                        \
    }                                                                       \
}

#define FifoT_GETCAPACITY_IMPL(T__, N__, SIZE_T__)                          \
SIZE_T__                                                                    \
N__##_getCapacity(N__ const* self)                                          \
{                                                                           \
    return self->capacity;                                                  \
}

#define FifoT_PUSH_IMPL(T__, N__, SIZE_T__)                                 \
bool                                                                        \
N__##_push(N__* self, T__ const* item)                                      \
{                                                                           \
    if (N__##_isFull(self))                                                 \
    {                                                                       \
        return false;                                                       \
    }                                                                       \
    DECL_UNUSED_VAR(const bool ok) =                                        \
        T__##_ctorCopy(&self->fifo[ self->last ], item);                    \
    Debug_ASSERT(ok);                                                       \
                                                                            \
    self->last = (self->last + 1) % self->capacity;                         \
    self->in++;                                                             \
    return true;                                                            \
}

#define FifoT_POP_IMPL(T__, N__)                                            \
bool                                                                        \
N__##_pop(N__* self)                                                        \
{                                                                           \
    if (N__##_isEmpty(self))                                                \
    {                                                                       \
        return false;                                                       \
    }                                                                       \
    T__##_dtor(&self->fifo[self->first]);                                   \
    self->first = (self->first + 1) % self->capacity;                       \
    self->out++;                                                            \
    return true;                                                            \
}

#define FifoT_GETFIRST_IMPL(T__, N__)                                       \
T__ const*                                                                  \
N__##_getFirst(N__ const* self)                                             \
{                                                                           \
    if (N__##_isEmpty(self))                                                \
    {                                                                       \
        return NULL;                                                        \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        return &self->fifo[self->first];                                    \
    }                                                                       \
}

#define FifoT_CLEAR_IMPL(T__, N__, SIZE_T__)                                \
void                                                                        \
N__##_clear(N__* self)                                                      \
{                                                                           \
    while (N__##_pop(self));                                                \
}

#define FifoT_CONST_APPLY_IMPL(T__, N__, SIZE_T__)                          \
SIZE_T__                                                                    \
N__##_constApply(N__ const* self,                                           \
                    N__##_applyFn fn,                                       \
                    void* context)                                          \
{                                                                           \
    SIZE_T__ i = 0;                                                         \
    SIZE_T__ size = N__##_getSize(self);                                    \
    bool cont = true;                                                       \
                                                                            \
    for (i = 0; i < size && cont; ++i)                                      \
    {                                                                       \
        SIZE_T__ index = (i + self->first) % self->capacity;                \
        cont = fn(context, &self->fifo[ index ], i);                        \
    }                                                                       \
    return i;                                                               \
}


#define FifoT_DEFINE(T__, N__, SIZE_T__)                                    \
         FifoT_CTOR_IMPL(T__, N__, SIZE_T__)                                \
         FifoT_DTOR_IMPL(T__, N__)                                          \
         FifoT_ISEMPTY_IMPL(T__, N__)                                       \
         FifoT_ISFULL_IMPL(T__, N__)                                        \
         FifoT_GETSIZE_IMPL(T__, N__, SIZE_T__)                             \
         FifoT_GETCAPACITY_IMPL(T__, N__, SIZE_T__)                         \
         FifoT_PUSH_IMPL(T__, N__, SIZE_T__)                                \
         FifoT_POP_IMPL(T__, N__)                                           \
         FifoT_GETFIRST_IMPL(T__, N__)                                      \
         FifoT_CLEAR_IMPL(T__, N__, SIZE_T__)                               \
         FifoT_CONST_APPLY_IMPL(T__, N__, SIZE_T__)
#endif
///@}
