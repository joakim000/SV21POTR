/**
 * @file test.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief For VG, all the requirements shall be fulfilled and the module shall be tested fully.
 *        Some suggestions for testing:
  *           1) Test the initialization of the buffer
 *            2) Test the empty/full buffer case
 *            3) Test read/peek/write when the buffer is empty
 *            4) Test read/peek/write when the buffer is full (an overwritten occurres when we write to a full buffer)
 *            5) Test read/peek/write when the buffer is not empty and full
 * 
 * @version 0.1
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../lib/unity/unity.h"
#include "cbuffer.h"

#ifndef I2
#define I2(x) (int i = 0; i < x; i++) 
#endif

void cbuffer_status();
void setUp()
{
    
   

}

void tearDown()
{
}

void WriteChars(char start, char end) {
    // Write some chars
    for (char i = start; i < end; i++)
        cbuffer_write(i);
}

void Test_cbuffer_clear() {
    WriteChars('A', 'D');   // Write some chars
    cbuffer_status();       // Print status
    cbuffer_clear();        // Call function

    // Should be clear
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_EQUAL(false, cbuffer_isfull());
}

void Test_cbuffer_isfull() {
    WriteChars('A', 'Q');
    TEST_ASSERT_EQUAL(true, cbuffer_isfull());

}


void TestBasics() {
    // Should be clear
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_EQUAL(false, cbuffer_isfull());
    
    // Write some chars
    WriteChars('A', 'I');
    TEST_ASSERT_EQUAL_UINT64('I'-'A', cbuffer_available());
    cbuffer_status();

    // Peek a char
    TEST_ASSERT_EQUAL_CHAR('A', cbuffer_peek());
    // Peek again, should be same
    TEST_ASSERT_EQUAL_CHAR('A', cbuffer_peek());

    // Read some chars
    int i;
    char c;
    for (i = 'A'; i < 'I'; i++) {
        c = cbuffer_read();
        TEST_ASSERT_EQUAL_CHAR(i, c);
    }
    cbuffer_status();
    TEST_ASSERT_EQUAL_UINT64(0, cbuffer_available());

}

int main(void)
{
    UNITY_BEGIN();
    setUp();
    cbuffer_status();

    RUN_TEST(TestBasics);
    RUN_TEST(Test_cbuffer_clear);
    RUN_TEST(Test_cbuffer_isfull);




    
    

    return UNITY_END();
}


