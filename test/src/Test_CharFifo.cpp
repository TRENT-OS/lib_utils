/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include <gtest/gtest.h>
#include <thread>

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


TEST_F(Test_CharFifo_extendedSetUp, head_chases_tail_concurrently)
{
    EXPECT_TRUE(CharFifo_isFull(&cf));

    // An arbitrary value that is big enough to cause multiple rounds of the
    // ring buffer and consumer/producer collisions at different places of the
    // ring buffer.
    const size_t iterations = 4096;

    std::thread producer
    {[this]
        {
            for(size_t i = kFifoSize; i < iterations; i++)
            {
                if(!CharFifo_push(&cf, (const char*) &i))
                {
                    // As producer and consumer running in parallel on the same
                    // priority to detect issues. If yield is not used, one of
                    // them will get all the computing time. And the other will
                    // starve. Yield is a pragmatic way to ensure computing time
                    // is shared.
                    std::this_thread::yield();
                    --i;
                }
            }
        }
    };

    std::thread consumer
    {[this]
        {
            for(size_t i = 0; i < iterations; i++)
            {
                char item;

                if(!CharFifo_isEmpty(&cf))
                {
                    ASSERT_EQ((char)i, CharFifo_getAndPop(&cf));
                }
                else
                {
                    // As producer and consumer running in parallel on the same
                    // priority to detect issues. If yield is not used, one of
                    // them will get all the computing time. And the other will
                    // starve. Yield is a pragmatic way to ensure computing time
                    // is shared.
                    std::this_thread::yield();
                    --i;
                }
            }
        }
    };

    producer.join();
    consumer.join();

    EXPECT_TRUE(CharFifo_isEmpty(&cf))
        << CharFifo_getSize(&cf);
}

TEST_F(Test_CharFifo, tail_chases_head_concurrently)
{
    EXPECT_TRUE(CharFifo_isEmpty(&cf));

    // An arbitrary value that is big enough to cause multiple rounds of the
    // ring buffer and consumer/producer collisions at different places of the
    // ring buffer.
    const size_t iterations = 4096;

    std::thread producer
    {[this]
        {
            for(size_t i = 0; i < iterations; i++)
            {
                if(!CharFifo_push(&cf, (const char*) &i))
                {
                    // As producer and consumer running in parallel on the same
                    // priority to detect issues. If yield is not used, one of
                    // them will get all the computing time. And the other will
                    // starve. Yield is a pragmatic way to ensure computing time
                    // is shared.
                    std::this_thread::yield();
                    --i;
                }
            }
        }
    };

    std::thread consumer
    {[this]
        {
            for(size_t i = 0; i < iterations; i++)
            {
                char item;

                if(!CharFifo_isEmpty(&cf))
                {
                    ASSERT_EQ((char)i, CharFifo_getAndPop(&cf));
                }
                else
                {
                    // As producer and consumer running in parallel on the same
                    // priority to detect issues. If yield is not used, one of
                    // them will get all the computing time. And the other will
                    // starve. Yield is a pragmatic way to ensure computing time
                    // is shared.
                    std::this_thread::yield();
                    --i;
                }
            }
        }
    };

    producer.join();
    consumer.join();

    EXPECT_TRUE(CharFifo_isEmpty(&cf))
        << CharFifo_getSize(&cf);
}
