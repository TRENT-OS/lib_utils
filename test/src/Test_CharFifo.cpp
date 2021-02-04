/*
 *  Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#include <gtest/gtest.h>

extern "C"
{
#include "lib_utils/CharFifo.h"
}

#define FIFO_SIZE 10
CharFifo cf;
const char c = 0;

class Test_CharFifo : public testing::Test
{
    protected:
};

static char fifoBuff[FIFO_SIZE];

TEST(Test_CharFifo, construction)
{
    bool ok = CharFifo_ctor(&cf, fifoBuff, FIFO_SIZE);
    ASSERT_TRUE(ok);
    ok = CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    ok = !CharFifo_isFull(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(size, 0);
    const char* first = CharFifo_getFirst(&cf);
    ASSERT_TRUE(first == NULL);
    size_t capacity = CharFifo_getCapacity(&cf);
    ASSERT_EQ(FIFO_SIZE, capacity);
}

TEST(Test_CharFifo, push_within_limits)
{
    for (unsigned int i = 0; i < FIFO_SIZE; i++)
    {
        char c = (char) i;
        bool ok = CharFifo_push(&cf, &c);
        ASSERT_TRUE(ok);
        ok = !CharFifo_isEmpty(&cf);
        ASSERT_TRUE(ok);
        size_t size = CharFifo_getSize(&cf);
        ASSERT_EQ(size, i + 1);
    }
}

TEST(Test_CharFifo, push_out_of_limits)
{
    bool ok = !CharFifo_push(&cf, &c);
    ASSERT_TRUE(ok);
    ok = !CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    ok = CharFifo_isFull(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(FIFO_SIZE, size);
    char c = (char) size;
    ok = CharFifo_forcedPush(&cf, &c);
    ASSERT_TRUE(ok);
}

TEST(Test_CharFifo, get_and_pop)
{
    for (unsigned int i = 0; i < FIFO_SIZE; i++)
    {
        char c = CharFifo_getAndPop(&cf);
        ASSERT_EQ(c, i + 1);
        size_t size = CharFifo_getSize(&cf);
        ASSERT_EQ(size, FIFO_SIZE - (i + 1));
        bool ok = !CharFifo_isFull(&cf);
        ASSERT_TRUE(ok);
    }
    bool ok = CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(size, 0);
}
