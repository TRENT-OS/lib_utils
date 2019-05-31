#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "lib_util/CharFifo.h"
/*
 * CUnit Test Suite
 */

#define FIFO_SIZE 10
CharFifo cf;
const char c = 0;

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

static char fifoBuff[FIFO_SIZE];

void test1()
{
    // test constructor
    bool ok = CharFifo_ctor(&cf, fifoBuff, FIFO_SIZE);
    CU_ASSERT(ok);
    ok = CharFifo_isEmpty(&cf);
    CU_ASSERT(ok);
    ok = !CharFifo_isFull(&cf);
    CU_ASSERT(ok);
    int size = CharFifo_getSize(&cf);
    CU_ASSERT_EQUAL(size, 0);
    char const* first = CharFifo_getFirst(&cf);
    CU_ASSERT_PTR_NULL(first);
    int capacity = CharFifo_getCapacity(&cf);
    CU_ASSERT_EQUAL(FIFO_SIZE, capacity);
}

void test2()
{
    // test push within the limits
    for (int i = 0; i < FIFO_SIZE; i++)
    {
        char c = (char) i;
        bool ok = CharFifo_push(&cf, &c);
        CU_ASSERT(ok);
        ok = !CharFifo_isEmpty(&cf);
        CU_ASSERT(ok);
        int size = CharFifo_getSize(&cf);
        CU_ASSERT_EQUAL(size, i + 1);
    }
}

void test3()
{
    // test push behind the limits
    bool ok = !CharFifo_push(&cf, &c);
    CU_ASSERT(ok);
    ok = !CharFifo_isEmpty(&cf);
    CU_ASSERT(ok);
    ok = CharFifo_isFull(&cf);
    CU_ASSERT(ok);
    int size = CharFifo_getSize(&cf);
    CU_ASSERT_EQUAL(FIFO_SIZE, size);
}

void test4()
{
    // test get and pop
    for (int i = 0; i < FIFO_SIZE; i++)
    {
        char c = CharFifo_getAndPop(&cf);
        CU_ASSERT_EQUAL(c, i);
        int size = CharFifo_getSize(&cf);
        CU_ASSERT_EQUAL(size, FIFO_SIZE - (i + 1));
        bool ok = !CharFifo_isFull(&cf);
        CU_ASSERT(ok);
    }
    bool ok = CharFifo_isEmpty(&cf);
    CU_ASSERT(ok);
    int size = CharFifo_getSize(&cf);
    CU_ASSERT_EQUAL(size, 0);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    /* Add a suite to the registry */
    pSuite = CU_add_suite("seos_libs/LibUtil/CharFifo",
                          init_suite,
                          clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite,
                             "test constructor",
                             test1)) ||
        (NULL == CU_add_test(pSuite,
                             "test push within the limits",
                             test2)) ||
        (NULL == CU_add_test(pSuite,
                             "test push behind the limits",
                             test3)) ||
        (NULL == CU_add_test(pSuite,
                             "test get and pop",
                             test4)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
