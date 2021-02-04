/*
 *  Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#include <gtest/gtest.h>

extern "C"
{
#include "lib_utils/CharFifo.h"
}

constexpr unsigned int kFifoSize = 10;

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
// Verify the CharFifo constructor
TEST_F(Test_CharFifo, construction)
{
    ASSERT_TRUE(CharFifo_isEmpty(&cf));
    ASSERT_FALSE(CharFifo_isFull(&cf));
    ASSERT_EQ(CharFifo_getSize(&cf), 0);
    ASSERT_TRUE(CharFifo_getFirst(&cf) == NULL);
    ASSERT_EQ(CharFifo_getCapacity(&cf), kFifoSize);
}

// Verify that once the CharFifo is filled, only forced pushs are possible
TEST_F(Test_CharFifo_extendedSetUp, push_out_of_limits)
{
    char c = 0;
    ASSERT_FALSE(CharFifo_push(&cf, &c));
    ASSERT_FALSE(CharFifo_isEmpty(&cf));
    ASSERT_TRUE(CharFifo_isFull(&cf));
    size_t size = CharFifo_getSize(&cf);
    ASSERT_EQ(kFifoSize, size);
    c = (char) size;
    ASSERT_TRUE(CharFifo_forcedPush(&cf, &c));
}

// Pop off the stored values and verify them against the filled entries
TEST_F(Test_CharFifo_extendedSetUp, get_and_pop)
{
    for (unsigned int i = 0; i < kFifoSize; i++)
    {
        size_t size_pre = CharFifo_getSize(&cf);
        ASSERT_EQ(size_pre, kFifoSize - i);
        ASSERT_EQ(CharFifo_getAndPop(&cf), i);
        size_t size_post = CharFifo_getSize(&cf);
        ASSERT_EQ(size_post, size_pre - 1);
        ASSERT_FALSE(CharFifo_isFull(&cf));
    }
    ASSERT_TRUE(CharFifo_isEmpty(&cf));
    ASSERT_EQ(CharFifo_getSize(&cf), 0);
}
