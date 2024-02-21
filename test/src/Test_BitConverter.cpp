/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include <gtest/gtest.h>

extern "C"
{
#include "lib_utils/BitConverter.h"
#include <stdint.h>
#include <limits.h>
}

class Test_BitConverter : public testing::Test
{
    protected:
};

TEST(Test_BitConverter, put)
{
    bool isBE = BitConverter_IS_BIG_ENDIAN();
    uint16_t value16 = 0x1234;
    uint32_t value32 = 0x12345678;
    uint64_t value64 = 0x1234567890123456;

    uint8_t array16[sizeof(value16)];
    uint8_t array32[sizeof(value32)];
    uint8_t array64[sizeof(value64)];

    BitConverter_putUint16BE(value16, array16);
    ASSERT_EQ(0x12, array16[0]);
    ASSERT_EQ(0x34, array16[1]);
    BitConverter_putUint16LE(value16, array16);
    ASSERT_EQ(0x12, array16[1]);
    ASSERT_EQ(0x34, array16[0]);
    BitConverter_putUint16(value16, array16);
    ASSERT_EQ(0x12, array16[isBE ? 0 : 1]);
    ASSERT_EQ(0x34, array16[isBE ? 1 : 0]);

    BitConverter_putUint32BE(value32, array32);
    ASSERT_EQ(0x12, array32[0]);
    ASSERT_EQ(0x34, array32[1]);
    ASSERT_EQ(0x56, array32[2]);
    ASSERT_EQ(0x78, array32[3]);
    BitConverter_putUint32LE(value32, array32);
    ASSERT_EQ(0x12, array32[3]);
    ASSERT_EQ(0x34, array32[2]);
    ASSERT_EQ(0x56, array32[1]);
    ASSERT_EQ(0x78, array32[0]);
    BitConverter_putUint32(value32, array32);
    ASSERT_EQ(0x12, array32[isBE ? 0 : 3]);
    ASSERT_EQ(0x34, array32[isBE ? 1 : 2]);
    ASSERT_EQ(0x56, array32[isBE ? 2 : 1]);
    ASSERT_EQ(0x78, array32[isBE ? 3 : 0]);

    BitConverter_putUint64BE(value64, array64);
    ASSERT_EQ(0x12, array64[0]);
    ASSERT_EQ(0x34, array64[1]);
    ASSERT_EQ(0x56, array64[2]);
    ASSERT_EQ(0x78, array64[3]);
    ASSERT_EQ(0x90, array64[4]);
    ASSERT_EQ(0x12, array64[5]);
    ASSERT_EQ(0x34, array64[6]);
    ASSERT_EQ(0x56, array64[7]);
    BitConverter_putUint64LE(value64, array64);
    ASSERT_EQ(0x12, array64[7]);
    ASSERT_EQ(0x34, array64[6]);
    ASSERT_EQ(0x56, array64[5]);
    ASSERT_EQ(0x78, array64[4]);
    ASSERT_EQ(0x90, array64[3]);
    ASSERT_EQ(0x12, array64[2]);
    ASSERT_EQ(0x34, array64[1]);
    ASSERT_EQ(0x56, array64[0]);
    BitConverter_putUint64(value64, array64);
    ASSERT_EQ(0x12, array64[isBE ? 0 : 7]);
    ASSERT_EQ(0x34, array64[isBE ? 1 : 6]);
    ASSERT_EQ(0x56, array64[isBE ? 2 : 5]);
    ASSERT_EQ(0x78, array64[isBE ? 3 : 4]);
    ASSERT_EQ(0x90, array64[isBE ? 4 : 3]);
    ASSERT_EQ(0x12, array64[isBE ? 5 : 2]);
    ASSERT_EQ(0x34, array64[isBE ? 6 : 1]);
    ASSERT_EQ(0x56, array64[isBE ? 7 : 0]);
}

TEST(Test_BitConverter, toUint)
{
    bool isBE = BitConverter_IS_BIG_ENDIAN();

    uint16_t value16 = 0;
    uint32_t value32 = 0;
    uint64_t value64 = 0;

    uint8_t array16[sizeof(value16)] = { 0x12, 0x34 };
    uint8_t array32[sizeof(value32)] = { 0x12, 0x34, 0x56, 0x78 };
    uint8_t array64[sizeof(value64)]
        = { 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56 };

    value16 = BitConverter_getUint16BE(array16);
    ASSERT_TRUE(0x1234);
    value16 = BitConverter_getUint16LE(array16);
    ASSERT_TRUE(0x3412);
    value16 = BitConverter_getUint16(array16);
    ASSERT_TRUE(value16 == isBE ? 0x1234 : 0x3412);

    value32 = BitConverter_getUint32BE(array32);
    ASSERT_TRUE(0x12345678);
    value32 = BitConverter_getUint32LE(array32);
    ASSERT_TRUE(0x78563412);
    value32 = BitConverter_getUint32(array32);
    ASSERT_TRUE(value32 == isBE ? 0x12345678 : 0x78563412);

    value64 = BitConverter_getUint64BE(array64);
    ASSERT_TRUE(0x1234567890123456);
    value64 = BitConverter_getUint64LE(array64);
    ASSERT_TRUE(0x5634129078563412);
    value64 = BitConverter_getUint64(array64);
    ASSERT_TRUE(value64 == isBE ? 0x1234567890123456 : 0x5634129078563412);
}
