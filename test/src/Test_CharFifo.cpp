/*
 * File:   Test_CharFifo
 * Author: cardone
 *
 * Created on 18-dic-2018, 20.17.35
 */

#include <gtest/gtest.h>

extern "C"
{
  #include "lib_util/CharFifo.h"
}

#define FIFO_SIZE 10
CharFifo cf;
const char c = 0;

class Test_CharFifo : public testing::Test {
protected:
};

TEST_F(Test_CharFifo, testExample) {
    bool ok = CharFifo_ctor(&cf, FIFO_SIZE);
    ASSERT_TRUE(ok);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
