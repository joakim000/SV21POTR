/** 
 * @brief SV21POTR Course B - C assignment 2
 * @author Joakim Odermalm
 */

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

#include <unity.h>
#include "cbuffer.h"


/**
 * @brief Test functions
 *          cbuffer_clear        X
 *          cbuffer_isfull       T F      
 *          cbuffer_peek         X 
 *          cbuffer_read         X 
 *          cbuffer_available    0 - 8  
 * 
 *       Test special cases
 *          read empty           X
 *          peek empty           X   
 *          write overflow       X
 */

// Test utils
void cbuffer_status();
void WriteChars(char start, char end) {
    // Write some chars
    for (char i = start; i < end; i++)
        cbuffer_write(i);
}

// Unity
void setUp() {}
void tearDown() {}


void Test_write_peek_read() {
    // Should be clear
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Init: cbuffer_available()");
    TEST_ASSERT_EQUAL_MESSAGE(false, cbuffer_isfull(), "Init: cbuffer_isfull()");
    
    // Write some chars
    WriteChars('A', 'I');
    TEST_ASSERT_EQUAL_UINT8_MESSAGE('I'-'A', cbuffer_available(), "Write done: cbuffer_available()");
    cbuffer_status();

    // Peek a char
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('A', cbuffer_peek(), "Once: cbuffer_peek()");
    // Peek again, should be same
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('A', cbuffer_peek(), "Again: cbuffer_peek()");

    // Read some chars
    int i;
    char c;
    for (i = 'A'; i < 'I'; i++) {
        c = cbuffer_read();
        TEST_ASSERT_EQUAL_CHAR_MESSAGE(i, c, "cbuffer_read()");
    }
    cbuffer_status();
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "All read: cbuffer_available()");

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
    cbuffer_status();
    TEST_ASSERT_EQUAL(true, cbuffer_isfull());
    cbuffer_read();
    TEST_ASSERT_EQUAL(false, cbuffer_isfull());
}

void Test_cbuffer_available() { 
    cbuffer_clear();
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Init");

    for (char i = 0; i < 8; i++) {
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i, cbuffer_available(), "Writing");
        cbuffer_write(i+65); // Printable chars
        cbuffer_status();

    }
    for (char i = 8; i >= 0; i--) {
        cbuffer_status();
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i, cbuffer_available(), "Reading");
        cbuffer_read();
    }

    // Read empty
    cbuffer_read();
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Exit");

}

void Test_case_empty() {
    cbuffer_clear();
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());

    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_peek());

    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_read());

}

void Test_case_write_overflow() {
    cbuffer_clear(); 
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());

    WriteChars('A', 'I'); // Fill buf exactly
    cbuffer_status();
    TEST_ASSERT_EQUAL(true, cbuffer_isfull());

    // One more write should evict first write (A)
    cbuffer_write('I'); 
    cbuffer_status();
    // Next read should then be B
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('B', cbuffer_read(), "A should be evicted"); 
}


int main(void)
{
    UNITY_BEGIN();
    setUp();
    cbuffer_status();

    RUN_TEST(Test_write_peek_read);
    RUN_TEST(Test_cbuffer_clear);
    RUN_TEST(Test_cbuffer_isfull);
    RUN_TEST(Test_cbuffer_available);
    RUN_TEST(Test_case_empty);
    RUN_TEST(Test_case_write_overflow);

    return UNITY_END();
}


