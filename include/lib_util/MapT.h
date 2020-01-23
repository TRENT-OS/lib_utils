/**
 * @addtogroup LibUtil
 * @{
 *
 * @file MapT.h
 *
 * @brief Map container template
 *
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 */
#if !defined(MAPT_H)
#define MAPT_H

#include "lib_util/VectorT.h"

/**
 * @file
 *
 * Map container template.
 * MapT is a preprocessor macro based template that can be used to
 * define an associative array of generic types. The association allows the
 * programmer to access a Value object via a Key reference.
 *
 * MapT requires that keys are unique.
 *
 * Any type can be used for key and value, provided they have a some methods
 * named in a standard way.
 *
 * Be K the type of the key then the following methods must be defined:
 *
 * @code
 * bool K_ctorCopy(K* target, K const* source);
 * bool K_ctorMove(K* target, K const* source);
 * bool K_assign(K* target, K const* source);
 * void K_dtor(K* object);
 * bool K_isEqual(K* a, K* b);
 * @endcode
 *
 * K_ctorCopy() must implements a copy constructor, i.e. given a valid object
 * then it constructs another object with the same value.
 *
 * K_assign() is the assignment operator. It is similar to the copy
 * constructor, but it handles only valid objects. The assign method assign the
 * value of the right parameter to the object referenced by the left parameter.
 *
 * K_dtor() is the object destructor.
 *
 * K_isEqual() checks for equality. If both parameters have the same value then
 * this function must return true.
 *
 * Be V the type of the value, the following methods must be defined:
 *
 * @code
 * bool V_ctorCopy(V* target, V const* source);
 * bool V_ctorMove(V* target, V const* source);
 * bool V_assign(V* target, V const* source);
 * void V_dtor(V* object);
 * @endcode
 * Methods have the same semantic of the ones required for K, applied to V.
 *
 * In order to use the container you cast the macro
 * @code
 * MapT_DECLARE(K,V,N);
 * @endcode
 * to declare all the container methods. This is likely to be done in a header
 * file.
 *
 * Then you have to cast the macro
 * @code
 * MapT_DEFINE(K,V,N);
 * @endcode
 * To define all the container methods. This is likely to be done in a C file.
 *
 * @note as of the current version of this component, access is implemented as
 *       a sequential search in the keys stored in the container. This means a
 *       complexity of O(N) where N is the number of the keys. Future versions
 *       of the container may employ more efficient algorithms at the cost of
 *       requiring additional methods for the key (e.g. comparison or hashing
 *       functions).
 */

#define MapT_GET_SIZE_BY_CAPACITY(T__, numItems)                         \
    (sizeof(N__##_Item) * numItems)

/**
 * @fn bool MapT_ctor(MapT* self, size_t capacity)
 *
 * Container constructor. If the object is properly constructed you have to
 * destroy it when you no longer need it via a call to #MapT_dtor().
 *
 * If the construction of an object fails, then no other method can be invoked
 * on such object.
 *
 * @param self a pointer to the object to construct.
 * @param capacity initialy capacity, dinamically mutable
 * @retval true the construction performed correctly.
 * @retval false the construction failed.
 */
#define MapT_CTOR_DECL(K__,V__,N__)          \
    bool N__##_ctor(N__* self, size_t capacity)

/**
 * @fn bool MapT_ctorStatic(MapT* self, void* buffer, size_t capacity)
 *
 * Container constructor. If the object is properly constructed you have to
 * destroy it when you no longer need it via a call to #MapT_dtor().
 *
 * If the construction of an object fails, then no other method can be invoked
 * on such object.
 *
 * @param self a pointer to the object to construct.
 * @param buffer memory buffer .
 * @param capacity initialy capacity, dinamically mutable
 * @retval true the construction performed correctly.
 * @retval false the construction failed.
 */
#define MapT_CTOR_STATIC_DECL(K__,V__,N__)          \
    bool N__##_ctorStatic(N__* self, void* buffer, size_t capacity)

/**
 * @fn bool MapT_ctorCopy(MapT* self)
 *
 * Container copy constructor. This method is similar to the constructor, but
 * on successful return the constructed object is initialized with values
 * copied from the given container.
 *
 * @param self a pointer to the object to construct.
 * @param a the container to use for initialization.
 * @retval true the construction performed correctly.
 * @retval false the construction failed.
 */
#define MapT_CTOR_COPY_DECL(K__,V__,N__)		\
	bool N__##_ctorCopy(N__* self, N__ const* orig)

/**
 * @fn void MapT_dtor(MapT* self)
 *
 * Container destructor. This method destroys the container and all the
 * contained objects - keys and values. After the destruction no method can be
 * invoked on the object.
 *
 * @param self.
 */
#define MapT_DTOR_DECL(K__,V__,N__)          \
    void N__##_dtor(N__* self)

/**
 * @fn bool MapT_insert(MapT* self, K const* key, V const* value)
 *
 * Inserts a new association in the container. Objects referenced by the
 * arguments key and value are copied so they can be disposed without affecting
 * the container contents and/or validity.
 *
 * @param self a pointer to the container.
 * @param key a pointer to the key object.
 * @param value a pointer to the value object.
 * @retval true the operation was successfully performed.
 * @retval false the operation failed either because the copy construction of
 *               key or value failed, or the system ran out of memory or
 *               another association with the same key is already in the
 *               container.
 */
#define MapT_INSERT_DECL(K__,V__,N__)        \
    bool N__##_insert(N__* self, K__ const* key, V__ const* value)

/**
 * @fn int MapT_getIndexOf(MapT const* self, K const* key)
 *
 * Retrieves the index of a key-value association. The index can be later used
 * to get the value or change it.
 *
 * The index is valid until one of the following function is invoked on the
 * container: #MapT_insert(), #MapT_remove(), #MapT_removeAt(), #MapT_clear(),
 * #MapT_dtor().
 *
 * @param self a pointer to the container itself.
 * @param key the key to search for.
 * @return an integer that holds the key,value association index. If the key is
 *         not found in the container then an integer less than 0 is returned.
 */
#define MapT_GETINDEXOF_DECL(K__, V__, N__)    \
    int N__##_getIndexOf(N__ const* self, K__ const* key)

/**
 * @fn void MapT_removeAt(MapT* self, int index)
 *
 * Removes a key,value association from the container.
 *
 * @param self a pointer to the container.
 * @param index the index of the association as returned from a successfull
 *              #MapT_getIndexOf(). It is an error to call this function with
 *              an invalid index. Also bear in mind that some calls invalidates
 *              the index, see #MapT_getIndexOf() for a detailed list of such
 *              functions.
 */
#define MapT_REMOVEAT_DECL(K__,V__,N__)        \
    void N__##_removeAt(N__* self, int index)

/**
 * @fn MapT_remove(MapT* self, K const* key)
 *
 * Removes a key,value association from the container.
 *
 * @param self a pointer to the container.
 * @param key the key of the association you want to remove. The key must be
 *            present in the container - it is an error attempting to remove
 *            with a key  not in the container.
 */
#define MapT_REMOVE_DECL(K__,V__,N__)        \
    bool N__##_remove(N__* self, K__ const* key)

/**
 * @fn V const* MapT_getValueAt(MapT const* self, int index)
 *
 * Retrieves the value of an association given the association index. The
 * returned pointer is valid until one of the following methods is invoked on
 * the container: #MapT_insert(), #MapT_removeAt(), #MapT_remove(),
 * #MapT_clear(), #MapT_dtor(). If you need to access the returned value
 * longer, then you have to make a copy of it.
 *
 * @param self a pointer to the container.
 * @param index a valid association index, as returned from #MapT_getIndexOf().
 * @return a pointer to the requested association.
 */
#define MapT_GETVALUEAT_DECL(K__,V__,N__)      \
    V__ const* N__##_getValueAt(N__ const* self, int index)

/**
 * @fn bool MapT_setValueAt(MapT* self, int index, V const* value)
 *
 * Changes the value of an association given the association index and the new
 * value.
 *
 * @param self a pointer to the container.
 * @param index a valid association index as returned from #MapT_getIndexOf().
 * @param value the new value. The pointed object will be copied into the
 *              container so that the original can be disposed without
 *              invalidating the container.
 * @retval true the operation was successfull.
 * @retval false the copy of the object pointed to by value failed, the
 *               container is left in the same state it was before this call.
 */
#define MapT_SETVALUEAT_DECL(K__,V__,N__)      \
    bool N__##_setValueAt(N__* self, int index, V__ const* value)

/**
 * @fn K const* MapT_getKeyAt(MapT const* self, int index)
 *
 * Retrieves the key of an association given a valid association index.
 *
 * @param self a pointer to the container.
 * @param index a valid association index as returned from #MapT_getIndexOf().
 * @return a pointer to the searched key. This reference is valid until one of
 *         the following call is performed on the container: #MapT_insert(),
 *         #MapT_removeAt(), #MapT_remove(), #MapT_clear(), #MapT_dtor(). If
 *         you need to access the returned value longer, then you have to make
 *         a copy of it.
 */

#define MapT_GETKEYAT_DECL(K__, V__, N__)      \
    K__ const* N__##_getKeyAt(N__ const* self, int index)

/**
 * @fn bool MapT_find(MapT const* self, K const* key)
 *
 * Checks whether an association with the given key is in the container or not.
 *
 * @param self a pointer to the container.
 * @param key a pointer to the key to search for.
 * @retval true the key is in the container.
 * @retval false the key is not in the container.
 */
#define MapT_FIND_DECL(K__,V__,N__)         \
    bool N__##_find(N__ const* self, K__ const* key)

/**
 * @fn bool MapT_isEmpty(MapT const* self)
 *
 * Checks for emptiness.
 *
 * @param self a pointer to the container.
 * @retval true the container is empty.
 * @retval false the container container one or more associations.
 */
#define MapT_ISEMPTY_DECL(K__,V__,N__)     \
    bool N__##_isEmpty(N__ const* self)

/**
 * @fn int MapT_getSize(MapT const* self);
 *
 * Returns the number of associations available in the container.
 *
 * @param self a pointer to the container.
 * @return the number of associations available in the container.
 */
#define MapT_GETSIZE_DECL(K__, V__, N__)  \
    int N__##_getSize(N__ const* self)

/**
 * @fn void MapT_clear(MapT* self)
 *
 * Clears the container by removing all the stored associations.
 *
 * @param self a pointer to the container.
 */
#define MapT_CLEAR_DECL(K__,V__,N__)     \
    void N__##_clear(N__* self)

/**
 * MapT template declaration macro. This macro has to be used to declare the
 * associative container. Where this macro will be instanced there will be the
 * container interface.
 *
 * @param K__ the key type.
 * @param V__ the value type.
 * @param N__ the name of the type that will provide the associative container
 *            on the given types for key and value.
 */
#define MapT_DECLARE(K__, V__, N__)                                         \
    typedef struct                                                          \
    {                                                                       \
        K__ key;                                                            \
        V__ value;                                                          \
    }                                                                       \
    N__##_Item;                                                             \
    void N__##_Item_dtor(N__##_Item* self);                                 \
    bool N__##_Item_ctorCopy(N__##_Item* self,                              \
                              N__##_Item const* orig);                      \
    bool N__##_Item_assign(N__##_Item* self,                                \
                            N__##_Item const* orig);                        \
    bool N__##_Item_ctorMove(N__##_Item* self,                              \
                            N__##_Item const* orig);                        \
    VectorT_DECLARE(N__##_Item, N__##_Impl, size_t);                        \
    typedef struct                                                          \
    {                                                                       \
        N__##_Impl mapImpl;                                                 \
    }                                                                       \
    N__;                                                                    \
    MapT_CTOR_DECL(K__,V__,N__);                                            \
    MapT_CTOR_STATIC_DECL(K__,V__,N__);                                     \
    MapT_CTOR_COPY_DECL(K__,V__,N__);                                       \
    MapT_DTOR_DECL(K__,V__,N__);                                            \
    MapT_INSERT_DECL(K__,V__,N__);                                          \
    MapT_REMOVEAT_DECL(K__,V__,N__);                                        \
    MapT_REMOVE_DECL(K__,V__,N__);                                          \
    MapT_GETINDEXOF_DECL(K__,V__,N__);                                      \
    MapT_GETVALUEAT_DECL(K__,V__,N__);                                      \
    MapT_SETVALUEAT_DECL(K__,V__,N__);                                      \
    MapT_GETKEYAT_DECL(K__, V__, N__);                                      \
    MapT_FIND_DECL(K__,V__,N__);                                            \
    MapT_ISEMPTY_DECL(K__,V__,N__);                                         \
    MapT_GETSIZE_DECL(K__, V__, N__);                                       \
    MapT_CLEAR_DECL(K__,V__,N__);

#define MapT_CTOR_IMPL(K__,V__,N__)                                         \
    bool                                                                    \
    N__##_ctor(N__* self, size_t capacity)                                  \
    {                                                                       \
        return N__##_Impl_ctor(&self->mapImpl, capacity);                   \
    }

#define MapT_CTOR_STATIC_IMPL(K__,V__,N__)                                  \
    bool                                                                    \
    N__##_ctorStatic(N__* self, void* buffer, size_t capacity)                    \
    {                                                                       \
        return N__##_Impl_ctorStatic(&self->mapImpl, buffer, capacity);     \
    }

#define MapT_CTOR_COPY_IMPL(K__,V__,N__)                                    \
	bool								    \
    N__##_ctorCopy(N__* self, N__ const* a)                                 \
    {									    \
	    return N__##_Impl_ctorCopy(&self->mapImpl, &a->mapImpl);        \
    }

#define MapT_DTOR_IMPL(K__,V__,N__)                                         \
    void                                                                    \
    N__##_dtor(N__* self)                                                   \
    {                                                                       \
        N__##_Impl_dtor(&self->mapImpl);                                    \
    }

#define MapT_INSERT_IMPL(K__,V__,N__)                                       \
		N__##_Item item_for_insert;                                             \
    bool N__##_insert(N__* self,                                            \
                       K__ const* key,                                      \
                       V__ const* value)                                    \
    {                                                                       \
        if (N__##_find(self, key))                                          \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        K__##_ctorCopy(&item_for_insert.key, key);                          \
        V__##_ctorCopy(&item_for_insert.value, value);                      \
        N__##_Impl_pushBackByPtr(&self->mapImpl, &item_for_insert);         \
        return true;                                                        \
    }

#define MapT_REMOVEAT_IMPL(K__,V__,N__)                                     \
		N__##_Item tmp_for_removeAt;                                          \
    void N__##_removeAt(N__* self, int index)                               \
    {                                                                       \
        Debug_ASSERT(index >= 0);                                          \
        int size = N__##_Impl_getSize(&self->mapImpl);                      \
        Debug_ASSERT(index < size);                                        \
        if (index !=  size-1)                                               \
        {                                                                   \
            tmp_for_removeAt = N__##_Impl_getElementAt(&self->mapImpl, size-1);\
            N__##_Impl_replaceElementAt(&self->mapImpl, index, &tmp_for_removeAt);       \
        }                                                                   \
        N__##_Impl_popBack(&self->mapImpl);                                 \
    }


#define MapT_REMOVE_IMPL(K__,V__,N__)                                       \
    bool N__##_remove(N__* self, K__ const* key)                            \
    {                                                                       \
        int index = N__##_getIndexOf(self, key);                            \
        if (index < 0)                                                      \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        N__##_removeAt(self, index);                                        \
        return true;                                                        \
    }

#define MapT_FIND_IMPL(K__,V__,N__)                                         \
    bool N__##_find(N__ const* self, K__ const* key)                        \
    {                                                                       \
        return N__##_getIndexOf(self, key) >= 0;                            \
    }

#define MapT_GETINDEXOF_IMPL(K__,V__,N__)                                   \
    int N__##_getIndexOf(N__ const* self, K__ const* key)                   \
    {                                                                       \
        int size = N__##_Impl_getSize(&self->mapImpl);                      \
        for (int i=0; i< size; i++)                                         \
        {                                                                   \
            N__##_Item const* item;                                         \
            item = N__##_Impl_getPtrToElementAt(&self->mapImpl, i);         \
            if (K__##_isEqual(key, &item->key))                             \
            {                                                               \
                return i;                                                   \
            }                                                               \
        }                                                                   \
        return -1;                                                          \
    }

#define MapT_GETVALUEAT_IMPL(K__,V__,N__)                                   \
    V__ const* N__##_getValueAt(N__ const* self, int index)                 \
    {                                                                       \
        N__##_Item const* item;                                             \
        item = N__##_Impl_getPtrToElementAt(&self->mapImpl, index);         \
        Debug_ASSERT(item >= 0);                                           \
        return &item->value;                                                \
    }

#define MapT_SETVALUEAT_IMPL(K__,V__,N__)                                   \
    N__##_Item newItem_for_setValueAt;                                      \
    bool N__##_setValueAt(N__* self,                                        \
                           int index,                                       \
                           V__ const* newValue)                             \
    {                                                                       \
        N__##_Item const* item;                                             \
        item = N__##_Impl_getPtrToElementAt(&self->mapImpl, index);         \
        Debug_ASSERT(item >= 0);                                           \
        if (K__##_ctorCopy(&newItem_for_setValueAt.key, &item->key))      \
        {                                                                   \
            if (V__##_ctorCopy(&newItem_for_setValueAt.value, newValue))    \
            {                                                               \
                if (N__##_Impl_replaceElementAt(&self->mapImpl,             \
                                                 index,                     \
                                                 &newItem_for_setValueAt))  \
                {                                                           \
                    return true;                                            \
                }                                                           \
                V__##_dtor(&newItem_for_setValueAt.value);                  \
            }                                                               \
            K__##_dtor(&newItem_for_setValueAt.key);                        \
        }                                                                   \
        return false;                                                       \
    }

#define MapT_GETKEYAT_IMPL(K__, V__, N__)                                   \
    K__ const* N__##_getKeyAt(N__ const* self, int index)                   \
    {                                                                       \
        Debug_ASSERT(index >= 0);                                          \
        N__##_Item const* item;                                             \
        item = N__##_Impl_getPtrToElementAt(&self->mapImpl, index);         \
        return &item->key;                                                  \
    }

#define MapT_ISEMPTY_IMPL(K__,V__,N__)                                      \
    bool N__##_isEmpty(N__ const* self)                                     \
    {                                                                       \
        return N__##_Impl_isEmpty(&self->mapImpl);                          \
    }

#define MapT_GETSIZE_IMPL(K__, V__, N__)                                    \
    int N__##_getSize(N__ const* self)                                      \
    {                                                                       \
        return N__##_Impl_getSize(&self->mapImpl);                          \
    }

#define MapT_CLEAR_IMPL(K__,V__,N__)                                        \
    void N__##_clear(N__* self)                                             \
    {                                                                       \
        N__##_Impl_clear(&self->mapImpl);                                   \
    }

#define MapT_Item_ctorCopy_IMPL(K__,V__,N__)                                \
    bool N__##_Item_ctorCopy(N__##_Item* self, N__##_Item const* src)       \
    {                                                                       \
        if (K__##_ctorCopy(&self->key, &src->key))                          \
        {                                                                   \
            if (V__##_ctorCopy(&self->value, &src->value))                  \
            {                                                               \
                return true;                                                \
            }                                                               \
            K__##_dtor(&self->key);                                         \
        }                                                                   \
        return false;                                                       \
    }

#define MapT_Item_assign_IMPL(K__,V__,N__)                                  \
    N__##_Item tmp_for_Item_assign;                                         \
    bool N__##_Item_assign(N__##_Item* self, N__##_Item const* src)         \
    {                                                                       \
        if (! N__##_Item_ctorCopy(&tmp_for_Item_assign, src))               \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        N__##_Item_dtor(self);                                              \
        *self = tmp_for_Item_assign;                                        \
        return true;                                                        \
    }

#define MapT_Item_ctorMove_IMPL(K__,V__,N__)                                \
    bool N__##_Item_ctorMove(N__##_Item* self, N__##_Item const* src)       \
    {                                                                       \
        if (K__##_ctorMove(&self->key, &src->key))                          \
        {                                                                   \
            if (V__##_ctorMove(&self->value, &src->value))                  \
            {                                                               \
                return true;                                                \
            }                                                               \
            K__##_dtor(&self->key);                                         \
        }                                                                   \
        return false;                                                       \
    }

#define MapT_Item_dtor_IMPL(K__,V__,N__)                                    \
    void N__##_Item_dtor(N__##_Item* self)                                  \
    {                                                                       \
        K__##_dtor(&self->key);                                             \
        V__##_dtor(&self->value);                                           \
    }

/**
 * MapT template definition macro. This macro has to be used to define the
 * associative container. Where this macro is instanced there will be the
 * container implementation.
 *
 * @param K__ the key type.
 * @param V__ the value type.
 * @param N__ the name of the type that will provide the associative container
 *            on the given types for key and value.
 */

#define MapT_DEFINE(K__,V__,N__)                \
    VectorT_DEFINE(N__##_Item, N__##_Impl, size_t)\
    MapT_Item_dtor_IMPL(K__,V__,N__)            \
    MapT_Item_ctorCopy_IMPL(K__,V__,N__)        \
    MapT_Item_assign_IMPL(K__,V__,N__)          \
    MapT_Item_ctorMove_IMPL(K__,V__,N__)        \
    MapT_CTOR_IMPL(K__,V__,N__)                 \
    MapT_CTOR_STATIC_IMPL(K__,V__,N__)          \
    MapT_CTOR_COPY_IMPL(K__,V__,N__)	        \
    MapT_DTOR_IMPL(K__,V__,N__)                 \
    MapT_INSERT_IMPL(K__,V__,N__)               \
    MapT_REMOVEAT_IMPL(K__,V__,N__)             \
    MapT_REMOVE_IMPL(K__,V__,N__)               \
    MapT_GETINDEXOF_IMPL(K__,V__,N__)           \
    MapT_GETVALUEAT_IMPL(K__,V__,N__)           \
    MapT_SETVALUEAT_IMPL(K__,V__,N__)           \
    MapT_GETKEYAT_IMPL(K__, V__, N__)           \
    MapT_FIND_IMPL(K__,V__,N__)                 \
    MapT_ISEMPTY_IMPL(K__,V__,N__)              \
    MapT_GETSIZE_IMPL(K__, V__, N__)            \
    MapT_CLEAR_IMPL(K__,V__,N__)


#if defined(DOXYGEN_SCAN)
// fake prototypes for doxygen use
bool MapT_ctor(MapT* self);
bool MapT_ctorCopy(MapT* self, MapT const* orig);
void MapT_dtor(MapT* self);
bool MapT_insert(MapT* self, K const* key, V const* value);
int MapT_getIndexOf(MapT const* self, K const* key);
void MapT_removeAt(MapT* self, int index);
MapT_remove(MapT* self, K const* key);
V const* MapT_getValueAt(MapT const* self, int index);
bool MapT_setValueAt(MapT* self, int index, V const* value);
K const* MapT_getKeyAt(MapT const* self, int index);
bool MapT_find(MapT const* self, K const* key);
bool MapT_isEmpty(MapT const* self);
int MapT_getSize(MapT const* self);;
void MapT_clear(MapT* self);
#endif

#endif
///@}

