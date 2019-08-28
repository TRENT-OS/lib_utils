/**
 * @addtogroup LibUtil
 * @{
 *
 * @file VectorT.h
 *
 * @brief Vector container template
 *
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 */
#if !defined(VECTORT_H)
#define VECTORT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "lib_mem/Memory.h"
#include "lib_debug/Debug.h"

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#if !defined(Vector_MAX_SIZE)
#define Vector_MAX_SIZE INT_MAX
#endif

/**
 * VectorT declaration macro. Use this in your header files.
 *
 * @param T the base type you want to define the vector type of.
 * @param N the name for the newly defined type.
 *
 * The type T must define the following operations:
 *
 * void T##_dtor(T*)               - type destructor
 * bool T##_ctorCopy(T*, T const*) - copy constructor
 * bool T##_ctorMove(T*, T const*) - move constructor
 * bool T##_assign(T*, T const*)   - assignment.
 */

#define VectorT_DECLARE(T, N, SIZE_T)                                       \
typedef struct                                                              \
{                                                                           \
    T* vector_;                                                             \
    SIZE_T size_;                                                           \
    SIZE_T nextFree_;                                                       \
    bool isStatic_;                                                         \
} N;                                                                        \
bool N##_ctor(N* v, SIZE_T defaultSize);                                    \
bool N##_ctorStatic(N* v, void* buffer, SIZE_T defaultSize);                \
bool N##_ctorCopy(N* v, N const* s);                                        \
void N##_dtor(N* v);                                                        \
N* N##_new(SIZE_T defaultSize);                                             \
void N##_del(N* v);                                                         \
bool N##_pushBack(N* v, T);                                                 \
bool N##_pushBackMove(N* v, T);                                             \
bool N##_pushBackByPtr(N* v, T const*);                                     \
bool N##_pushBackMoveByPtr(N* v, T const*);                                 \
T N##_getFront(N const* v);                                                 \
T N##_getBack(N const* v);                                                  \
void N##_popBack(N* v);                                                     \
T N##_getElementAt(N const* v, SIZE_T n);                                   \
T const* N##_getPtrToElementAt(N const* v, SIZE_T n);                       \
bool N##_replaceElementAt(N* v, SIZE_T n, T newElement);                    \
SIZE_T N##_getSize(N const* v);                                             \
bool N##_isEmpty(N const* v);                                               \
void N##_clear(N* v);                                                       \
bool N##_resizeIfNeeded(N* v)

/**
 * @fn bool VectorT_ctor( VectorT* v )
 *
 * Vector container constructor.
 *
 * @param v a pointer to the container.
 * @retval true the constructor completed successfully.
 * @retval false the constructor failed.
 */

/**
 * @fn bool VectorT_ctorCopy( VectorT* v, VectorT const* s )
 *
 * Copy constructor. If the copy construction succeeds then all the element in
 * 's' are copied and added to 'v'. The copy preserves the ordering.
 *
 * @param v a pointer to the container to construct.
 * @param s a pointer to the container to copy from.
 * @retval true the constructor completed successfully.
 * @retval false the constructor failed.
 */

/**
 * @fn void VectorT_dtor( VectorT* v )
 *
 * Destructor.
 * After destruction the object is no longer valid.
 *
 * @param v a pointer to the container.
 */

/**
 * @fn VectorT* VectorT_new( void )
 *
 * Dynamic object creation. This method is a shortcut for dynamic allocation of
 * the VectorT space followed by object constrution. You are expected to call
 * #VectorT_delete() to dispose an container allocated via #VectorT_new().
 *
 * @return a pointer to an empty container or NULL if the system ran out of
 *         memory.
 */

/**
 * @fn void VectorT_delete( VectorT* v )
 *
 * Dynamic object destruction. This method is a shortcut for dynamic
 * deallocation - it invokes the container destructor followed by a release of
 * the memory where the container was.
 *
 * @param v a pointer to the container to free.
 */

/**
 * @fn bool VectorT_pushBack( VectorT* v, T item )
 *
 * Adds an object to the end of the container. Note that the object is passed
 * by value (not by reference), but it is copied with its copy constructor
 * before addition into the container.
 *
 * @param v a pointer to the container.
 * @param item an object to add at the end of the container.
 * @retval true if the operation has been completed successfully.
 * @retval false if the operation failed. In this case the container is left in
 *               the same state it was prior to this call.
 */

/**
 * @fn T VectorT_getFront( VectorT const* v )
 *
 * Retrieves the first element in the vector. This element is at position 0.
 * The object is copied and the copy is returned. It is up to the caller to
 * invoke the destructor on the returned value.
 *
 * @note it is an error to call this method on an empty vector.
 * @note there is no way to determine whether the copy construction of the
 *       returned value fails.
 *
 * @param v a pointer to the container.
 * @return a copy of the object at position 0.
 */

/**
 * @fn T VectorT_getBack( VectorT const* v )
 *
 * Retrieves the last element in the vector. The object is copied and the copy
 * is returned. It is up to the caller to invoke the destructor on the returned
 * value.
 *
 * @note it is an error to call this method on an empty vector.
 * @note there is no way to determine whether the copy construction of the
 *       returned value fails.
 *
 * @param v a pointer to the container.
 * @return a copy of the object at last position.
 */

/**
 * @fn void VectorT_popBack( VectorT* v )
 *
 * Removes and discards the object at the last position in the vector.
 *
 * @param v a pointer to the container.
 */

/**
 * @fn T VectorT_getElementAt( VectorT const* v, int n )
 *
 * Retrieves a copy of the element at the given position.
 *
 * @note it is an error to invoke this method on a non existing position.
 *
 * @param v a pointer to the vector.
 * @param n the position to retrieve.
 * @return a copy of the object at position 'n' in vector 'v'. It is up to the
 *         caller to invoke the destructor on the returned object.
 */

/**
 * @fn T const* VectorT_getPtrToElementAt( VectorT const* v, int n )
 *
 * Retrieves the reference to the object at a given position. The reference is
 * valid until one of the following method is invoked on the vector:
 * #VectorT_dtor(), #VectorT_delete() (if applicable), #VectorT_pushBack(),
 * #VectorT_popBack(), #VectorT_clear().
 *
 * @param v a pointer to the vector.
 * @param n the index you want to retrieve. It is an error to require a non
 *          existing or invalid index.
 * @return a pointer to the item at position 'n'.
 */

/**
 * @fn bool VectorT_replaceElementAt( VectorT* v, int n, T newElement )
 *
 * Replace the element at the given position.
 *
 * @param v a pointer to the vector.
 * @param n the position you want to replace. It is an error to specify a non
 *          existing position or an invalid index (negative).
 * @param newElement the new value to assign to the element at position n.
 * @retval true the operation has been successfully completed.
 * @retval false the operation failed.
 */

/**
 * @fn int VectorT_getSize( VectorT const* v )
 *
 * Retrieves the number of elements contained in the vector.
 *
 * @param v a pointer to the vector.
 * @return the number of elements in the container.
 */

/**
 * @fn bool VectorT_isEmpty( VectorT const* v )
 *
 * Emptiness checker.
 *
 * @param v a pointer to the vector.
 * @retval true if the container is empty.
 * @retval false if the vector contains one or more items.
 */

/**
 * @fn void VectorT_clear( VectorT* v )
 *
 * Empty the container. This function removes all the elements (if any) stored
 * in the container.
 *
 * @param v a pointer to the vector.
 */

#define VectorT_GET_SIZE_BY_CAPACITY(T__, numItems)                         \
    (sizeof(T__) * numItems)

#define VectorT_ASSERTInvariants(T__, N__, V__, SIZE_T)                     \
    do                                                                      \
    {                                                                       \
        N__ const* v__ = (V__); /* single evaluation */                     \
        Debug_ASSERT(v__->vector_ != NULL);                                 \
        Debug_ASSERT(v__->nextFree_ >= 0);                                  \
        Debug_ASSERT(v__->size >= v__->nextFree_);                          \
        for (SIZE_T i = 0; i < v__->nextFree_; ++i)                         \
        {                                                                   \
            T__##_ASSERTInvariants(&v__->vector_[i]);                       \
        }                                                                   \
    }                                                                       \
    while(0)

#if defined(Memory_Config_STATIC)
#   define VectorT_DEFINE_CTOR(T, N, SIZE_T)
#   define VectorT_DEFINE_CTOR_COPY(T, N, SIZE_T)
#   define VectorT_DEFINE_NEW(T, N, SIZE_T)
#   define VectorT_DEFINE_DEL(T, N, SIZE_T)
#   define VectorT_DEFINE_RESIZE_IF_NEEDED(T, N, SIZE_T)                    \
    bool N##_resizeIfNeeded(N* v)                                           \
    {                                                                       \
        return (v->nextFree_ < v->size_)                                    \
    }
#else
#   define VectorT_DEFINE_CTOR(T, N, SIZE_T)                                \
    bool N##_ctor(N* v, SIZE_T defaultSize)                                 \
    {                                                                       \
        v->vector_ = Memory_alloc(sizeof(T) * defaultSize);                 \
        if (v->vector_ == NULL)                                             \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        v->size_        = defaultSize;                                      \
        v->nextFree_    = 0;                                                \
        v->isStatic_    = false;                                            \
        return true;                                                        \
    }
#   define VectorT_DEFINE_CTOR_COPY(T, N, SIZE_T)                           \
    bool N##_ctorCopy(N* v, N const* s)                                     \
    {                                                                       \
        v->vector_ = Memory_alloc(s->size_ * sizeof(T));                    \
        if (v->vector_ == NULL)                                             \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        v->size_ = s->size_;                                                \
        v->nextFree_ = s->nextFree_;                                        \
                                                                            \
        for (SIZE_T i = 0; i < v->nextFree_; ++i)                           \
        {                                                                   \
            if (!T##_ctorCopy(&v->vector_[i], &s->vector_[i]))              \
            {                                                               \
                /* rollback */                                              \
                for (--i ; i>= 0; --i)                                      \
                {                                                           \
                    T##_dtor(&v->vector_[i]);                               \
                }                                                           \
                Memory_free(v->vector_);                                    \
                return false;                                               \
            }                                                               \
        }                                                                   \
        return true;                                                        \
    }
#   define VectorT_DEFINE_NEW(T, N, SIZE_T)                                 \
    N* N##_new(SIZE_T defaultSize)                                          \
    {                                                                       \
        N* object = Memory_alloc(sizeof(N));                                \
                                                                            \
        if (object != NULL)                                                 \
        {                                                                   \
            if (!N##_ctor(object, defaultSize))                             \
            {                                                               \
                Memory_free(object);                                        \
                object = NULL;                                              \
            }                                                               \
        }                                                                   \
        return object;                                                      \
    }
#   define VectorT_DEFINE_DEL(T, N, SIZE_T)                                 \
    void N##_del(N* v)                                                      \
    {                                                                       \
        if (v)                                                              \
        {                                                                   \
            N##_dtor(v);                                                    \
            Memory_free(v);                                                 \
        }                                                                   \
    }
#   define VectorT_DEFINE_RESIZE_IF_NEEDED(T, N, SIZE_T)                    \
    bool N##_resizeIfNeeded(N* v)                                           \
    {                                                                       \
        bool retval = false;                                                \
                                                                            \
        if (v->nextFree_ < v->size_)                                        \
        {                                                                   \
            retval = true;                                                  \
        }                                                                   \
        else if (v->isStatic_)                                              \
        {                                                                   \
            retval = false;                                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            T* newVector = NULL;                                            \
            SIZE_T newSize = 2 * v->size_;                                  \
                                                                            \
            if (newSize <= Vector_MAX_SIZE)                                 \
                newVector = Memory_alloc(newSize * sizeof(T));              \
                                                                            \
            if (newVector == NULL)                                          \
            {                                                               \
                retval = false;                                             \
            }                                                               \
            else                                                            \
            {                                                               \
                SIZE_T i = 0;                                               \
                retval = true;                                              \
                                                                            \
                for (; i < v->size_ && retval; ++i)                         \
                {                                                           \
                    retval = T##_ctorCopy(&newVector[i], &v->vector_[i]);   \
                    T##_dtor(&v->vector_[i]);                               \
                }                                                           \
                for (; i > 0 && !retval; i--)                               \
                {                                                           \
                    T##_dtor(&newVector[i]);                                \
                }                                                           \
                Memory_free(v->vector_);                                    \
                v->vector_  = newVector;                                    \
                v->size_    = newSize;                                      \
            }                                                               \
        }                                                                   \
        return retval;                                                      \
    }
#endif

#define VectorT_DEFINE(T, N, SIZE_T)                                        \
                                                                            \
    VectorT_DEFINE_CTOR(T, N, SIZE_T)                                       \
                                                                            \
    bool N##_ctorStatic(N* v, void* buffer, SIZE_T defaultSize)             \
    {                                                                       \
        v->vector_ = buffer;                                                \
        if (v->vector_ == NULL)                                             \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        v->size_        = defaultSize;                                      \
        v->nextFree_    = 0;                                                \
        v->isStatic_    = true;                                             \
        return true;                                                        \
    }                                                                       \
                                                                            \
    VectorT_DEFINE_CTOR_COPY(T, N, SIZE_T)                                  \
                                                                            \
    void N##_dtor(N* v)                                                     \
    {                                                                       \
        for (SIZE_T i = 0; i < v->nextFree_; ++i)                           \
        {                                                                   \
            T##_dtor(&v->vector_[i]);                                       \
        }                                                                   \
        if (!v->isStatic_)                                                  \
        {                                                                   \
            Memory_free(v->vector_);                                        \
        }                                                                   \
    }                                                                       \
                                                                            \
    VectorT_DEFINE_NEW(T, N, SIZE_T)                                        \
                                                                            \
    VectorT_DEFINE_DEL(T, N, SIZE_T)                                        \
                                                                            \
    bool N##_pushBackByPtr(N* v, T const* item)                             \
    {                                                                       \
        bool retval = true;                                                 \
                                                                            \
        if (N##_resizeIfNeeded(v) &&                                        \
            T##_ctorCopy(&v->vector_[v->nextFree_], item))                  \
        {                                                                   \
            v->nextFree_++;                                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            retval = false;                                                 \
        }                                                                   \
        return retval;                                                      \
    }                                                                       \
                                                                            \
    bool N##_pushBack(N* v, T item)                                         \
    {                                                                       \
        bool retval = true;                                                 \
                                                                            \
        if (N##_resizeIfNeeded(v) &&                                        \
            T##_ctorCopy(&v->vector_[v->nextFree_], &item))                 \
        {                                                                   \
            v->nextFree_++;                                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            retval = false;                                                 \
        }                                                                   \
        return retval;                                                      \
    }                                                                       \
                                                                            \
    bool N##_pushBackMove(N* v, T item)                                     \
    {                                                                       \
        bool retval = true;                                                 \
                                                                            \
        if (N##_resizeIfNeeded(v) &&                                        \
            T##_ctorMove(&v->vector_[v->nextFree_], &item))                 \
        {                                                                   \
            v->nextFree_++;                                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            retval = false;                                                 \
        }                                                                   \
        return retval;                                                      \
    }                                                                       \
                                                                            \
    bool N##_pushBackMoveByPtr(N* v, T const* item)                         \
    {                                                                       \
        bool retval = true;                                                 \
                                                                            \
        if (N##_resizeIfNeeded(v) &&                                        \
            T##_ctorMove(&v->vector_[v->nextFree_], item))                  \
        {                                                                   \
            v->nextFree_++;                                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            retval = false;                                                 \
        }                                                                   \
        return retval;                                                      \
    }                                                                       \
                                                                            \
    void N##_popBack(N* v)                                                  \
    {                                                                       \
        if (v->nextFree_ > 0)                                               \
        {                                                                   \
            T##_dtor(&v->vector_[v->nextFree_-1]);                          \
            --v->nextFree_;                                                 \
        }                                                                   \
    }                                                                       \
                                                                            \
    T N##_getFront(N const* v)                                              \
    {                                                                       \
        Debug_ASSERT(v->nextFree_ > 0);                                     \
        T item;                                                             \
        T##_ctorCopy(&item, &v->vector_[0]);                                \
        return item;                                                        \
    }                                                                       \
                                                                            \
    T N##_getBack(N const* v)                                               \
    {                                                                       \
        Debug_ASSERT(v->nextFree_ > 0);                                     \
        T item;                                                             \
        T##_ctorCopy(&item, &v->vector_[v->nextFree_-1]);                   \
        return item;                                                        \
    }                                                                       \
                                                                            \
    T N##_getElementAt(N const* v, SIZE_T n)                                \
    {                                                                       \
        Debug_ASSERT(n >= 0 && n < v->nextFree_);                           \
        T item;                                                             \
        T##_ctorCopy(&item, &v->vector_[n]);                                \
        return item;                                                        \
    }                                                                       \
                                                                            \
    T const* N##_getPtrToElementAt(N const* v, SIZE_T n)                    \
    {                                                                       \
        Debug_ASSERT(n >= 0 && n < v->nextFree_);                           \
        return &v->vector_[n];                                              \
    }                                                                       \
                                                                            \
    bool N##_replaceElementAt(N* v, SIZE_T n, T newElement)                 \
    {                                                                       \
        Debug_ASSERT(n >= 0 && n < v->nextFree_);                           \
        return T##_assign(&v->vector_[n], &newElement);                     \
    }                                                                       \
                                                                            \
    SIZE_T N##_getSize(N const* v)                                          \
    {                                                                       \
        return v->nextFree_;                                                \
    }                                                                       \
                                                                            \
    bool N##_isEmpty(N const* v)                                            \
    {                                                                       \
        return v->nextFree_ == 0;                                           \
    }                                                                       \
                                                                            \
    void N##_clear(N* v)                                                    \
    {                                                                       \
        for (SIZE_T i = 0; i < v->nextFree_; ++i)                           \
        {                                                                   \
            T##_dtor(&v->vector_[i]);                                       \
        }                                                                   \
        v->nextFree_ = 0;                                                   \
    }                                                                       \
    VectorT_DEFINE_RESIZE_IF_NEEDED(T, N, SIZE_T)

#if defined(__cplusplus)
}
#endif

#if defined( DOXYGEN_SCAN )
bool VectorT_ctor( VectorT* v );
bool VectorT_ctorCopy( VectorT* v, VectorT const* s );
void VectorT_dtor( VectorT* v );
VectorT* VectorT_new( void );
void VectorT_delete( VectorT* v );
bool VectorT_pushBack( VectorT* v, T item );
T VectorT_getFront( VectorT const* v );
T VectorT_getBack( VectorT const* v );
void VectorT_popBack( VectorT* v );
T VectorT_getElementAt( VectorT const* v, int n );
T const* VectorT_getPtrToElementAt( VectorT const* v, int n );
bool VectorT_replaceElementAt( VectorT* v, int n, T newElement );
int VectorT_getSize( VectorT const* v );
bool VectorT_isEmpty( VectorT const* v );
void VectorT_clear( VectorT* v );
#endif

#endif
///@}

