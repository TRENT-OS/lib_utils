/*
 *  Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#include <gtest/gtest.h>

extern "C"
{
#include "lib_utils/CharFifo.h"
}

constexpr unsigned int kFifoSize = 10;

static void fillFifo(CharFifo* self, const size_t fifoSize)
{
    for (unsigned int i = 0; i < fifoSize; i++)
    {
        size_t size_pre = CharFifo_getSize(self);
        ASSERT_TRUE(CharFifo_push(self, (const char*) &i));
        ASSERT_FALSE(CharFifo_isEmpty(self));
        size_t size_post = CharFifo_getSize(self);
        ASSERT_EQ(size_post, size_pre + 1);
    }
    ASSERT_TRUE(CharFifo_isFull(self));
}

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

class Test_CharFifo_extendedSetUp : public Test_CharFifo
{
    protected:
        void SetUp()
        {
            Test_CharFifo::SetUp();
            fillFifo(&cf, sizeof(fifoBuff));
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

TEST_F(Test_CharFifo_extendedSetUp, push_out_of_limits)
{
    char c = 0;
    bool ok = !CharFifo_push(&cf, &c);
    ASSERT_TRUE(ok);
    ok = !CharFifo_isEmpty(&cf);
    ASSERT_TRUE(ok);
    ok = CharFifo_isFull(&cf);
    ASSERT_TRUE(ok);
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(kFifoSize, size);
    c = (char) size;
    ok = CharFifo_forcedPush(&cf, &c);
    ASSERT_TRUE(ok);
}

TEST_F(Test_CharFifo_extendedSetUp, get_and_pop)
{
    for (unsigned int i = 0; i < kFifoSize; i++)
    {
        char c = CharFifo_getAndPop(&cf);
        ASSERT_EQ(c, i);
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
