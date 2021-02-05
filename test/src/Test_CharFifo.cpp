/*
 *  Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#include <gtest/gtest.h>

extern "C"
{
#include "lib_utils/CharFifo.h"
}

constexpr unsigned int kFifoSize = 10;
const char c = 0;

class Test_CharFifo : public testing::Test
{
    protected:
        CharFifo cf;
        char fifoBuff[kFifoSize];
        void SetUp()
        {
            ASSERT_TRUE(CharFifo_ctor(&cf, fifoBuff, kFifoSize));
        }

        void TearDown()
        {
            CharFifo_dtor(&cf);
        }
};

TEST_F(Test_CharFifo, construction)
{
    bool ok = CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    ok = !CharFifo_isFull(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(size, 0);
    const char* first = CharFifo_getFirst(&cf);
    ASSERT_TRUE(first == NULL);
    size_t capacity = CharFifo_getCapacity(&cf);
    ASSERT_EQ(kFifoSize, capacity);
}

TEST_F(Test_CharFifo, push_within_limits)
{
    for (unsigned int i = 0; i < kFifoSize; i++)
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

TEST_F(Test_CharFifo, push_out_of_limits)
{
    bool ok = !CharFifo_push(&cf, &c);
    ASSERT_TRUE(ok);
    ok = !CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    ok = CharFifo_isFull(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(kFifoSize, size);
    char c = (char) size;
    ok = CharFifo_forcedPush(&cf, &c);
    ASSERT_TRUE(ok);
}

TEST_F(Test_CharFifo, get_and_pop)
{
    for (unsigned int i = 0; i < kFifoSize; i++)
    {
        char c = CharFifo_getAndPop(&cf);
        ASSERT_EQ(c, i + 1);
        size_t size = CharFifo_getSize(&cf);
        ASSERT_EQ(size, kFifoSize - (i + 1));
        bool ok = !CharFifo_isFull(&cf);
        ASSERT_TRUE(ok);
    }
    bool ok = CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(size, 0);
}
