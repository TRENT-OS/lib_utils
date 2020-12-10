/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include <gtest/gtest.h>

extern "C"
{
#include "lib_util/RleCompressor.h"
#include <stdint.h>
#include <limits.h>
}

class Test_RleCompressor : public testing::Test
{
    protected:
};

TEST(Test_RleCompressor, compress_pos)
{
    uint8_t inbuf[32] = {0}, outbuf[32];
    uint8_t* static_buf = outbuf;
    size_t len = 0;

    // Output buffer can hold ONLY the header; actual input is 0 but this should
    // be OK, then we just get a header indicating a zero-length result after
    // decompression
    ASSERT_EQ(OS_SUCCESS,
              RleCompressor_compress(0, inbuf, 7, &len,  &static_buf));
}

TEST(Test_RleCompressor, compress_neg)
{
    uint8_t inbuf[32] = {0}, outbuf[32];
    uint8_t* alloc_buf, *static_buf = outbuf;
    size_t len = 0;

    // Input buffer is NULL
    ASSERT_EQ(OS_ERROR_INVALID_PARAMETER,
              RleCompressor_compress(sizeof(inbuf), NULL, 0, &len, &alloc_buf));

    // Output len is NULL
    ASSERT_EQ(OS_ERROR_INVALID_PARAMETER,
              RleCompressor_compress(sizeof(inbuf), inbuf, 0, NULL, &alloc_buf));

    // OUtput buffer is NULL
    ASSERT_EQ(OS_ERROR_INVALID_PARAMETER,
              RleCompressor_compress(sizeof(inbuf), inbuf, 0, &len, NULL));

    // Output buffer can hold header, but not more
    ASSERT_EQ(OS_ERROR_ABORTED,
              RleCompressor_compress(sizeof(inbuf), inbuf, 7, &len,  &static_buf));

    // Output buffer cannot even hold the header
    ASSERT_EQ(OS_ERROR_BUFFER_TOO_SMALL,
              RleCompressor_compress(0, inbuf, 6, &len,  &static_buf));
}

TEST(Test_RleCompressor, decompress_neg)
{
    uint8_t inbuf[32] = {0}, outbuf[32];
    uint8_t* alloc_buf, *static_buf = outbuf;
    size_t len;

    // Input buffer is NULL
    ASSERT_EQ(OS_ERROR_INVALID_PARAMETER,
              RleCompressor_decompress(sizeof(inbuf), NULL, 0, &len, &alloc_buf));

    // Input len is NULL
    ASSERT_EQ(OS_ERROR_INVALID_PARAMETER,
              RleCompressor_decompress(sizeof(inbuf), inbuf, 0, NULL, &alloc_buf));

    // Output buffer is NULL
    ASSERT_EQ(OS_ERROR_INVALID_PARAMETER,
              RleCompressor_decompress(sizeof(inbuf), inbuf, 0, &len, NULL));

    // Input buffer buffer doesn't even have space for header
    ASSERT_EQ(OS_ERROR_BUFFER_TOO_SMALL,
              RleCompressor_decompress(6, inbuf, sizeof(outbuf), &len,  &static_buf));

    // Compress something, then mess up the header
    ASSERT_EQ(OS_SUCCESS,
              RleCompressor_compress(sizeof(inbuf), inbuf, sizeof(outbuf), &len,
                                     &static_buf));
    static_buf = inbuf;

    // Mess up the "RLE" header
    outbuf[0] ^= 0xFF;
    ASSERT_EQ(OS_ERROR_INVALID_STATE,
              RleCompressor_decompress(len, outbuf, sizeof(inbuf), &len, &static_buf));
    outbuf[0] ^= 0xFF;

    // Mess up the decompressed size in the header
    outbuf[4]  = 0xFF;
    ASSERT_EQ(OS_ERROR_BUFFER_TOO_SMALL,
              RleCompressor_decompress(len, outbuf, sizeof(inbuf), &len, &static_buf));
    outbuf[4] = 0x00;

    // Mess up the length of the first symbol; so it would fit from the encoded
    // length of the total block, but then doesn't fit when decoding the first
    // symbol
    outbuf[7] = 0xFF;
    ASSERT_EQ(OS_ERROR_ABORTED,
              RleCompressor_decompress(len, outbuf, sizeof(inbuf), &len, &static_buf));
}

TEST(Test_RleCompressor, compress_decompress)
{
    uint8_t inbuf[32] = {0}, outbuf[32];
    uint8_t* alloc_buf, *static_buf = outbuf;
    size_t len;

    // Compress into a static buf, then decompress into an allocated buf
    ASSERT_EQ(OS_SUCCESS,
              RleCompressor_compress(sizeof(inbuf), inbuf, sizeof(outbuf), &len,
                                     &static_buf));
    ASSERT_EQ(OS_SUCCESS,
              RleCompressor_decompress(len, static_buf, 0, &len, &alloc_buf));
    ASSERT_EQ(len, sizeof(inbuf));
    ASSERT_EQ(0, memcmp(alloc_buf, inbuf, sizeof(inbuf)));
    free(alloc_buf);

    // Compress into an allocated buf, then decompress into static buf
    ASSERT_EQ(OS_SUCCESS,
              RleCompressor_compress(sizeof(inbuf), inbuf, 0, &len, &alloc_buf));
    ASSERT_EQ(OS_SUCCESS,
              RleCompressor_decompress(len, alloc_buf, sizeof(outbuf), &len, &static_buf));
    ASSERT_EQ(len, sizeof(inbuf));
    ASSERT_EQ(0, memcmp(static_buf, inbuf, sizeof(inbuf)));
    free(alloc_buf);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}